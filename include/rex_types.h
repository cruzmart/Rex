#pragma once

#include <memory>
#include <string>
#include <vector>
#include <iostream>

namespace rex {

// -------------------------------------------------
// Type kinds supported by Rex
// -------------------------------------------------
enum class TypeKind {
  
    Primitive,

    Range,   // result of a..b
    Array,   // fixed-size array: T[n]
    Slice,   // dynamic-size slice: T[]
    Tuple,   // tuple: (T1, T2, ...)


    Void,    // statements / no value
    Error,    // type-checking failure

    Named,      //for type aliasing 

    Function

};

// -------------------------------------------------
// Type representation
// -------------------------------------------------
struct Type {
    TypeKind fundamental_kind;

    // type alias
    std::string alias;

    // For Array / Slice
    std::shared_ptr<Type> array_type_prim; // array type fundemntally
    int array_size = -1;           // >=0 only for Array

    // For Tuple
    std::vector<std::shared_ptr<Type>> tuple_types;

    // ----------------------
    // Constructors
    // ----------------------
    Type() : fundamental_kind(TypeKind::Error) {}               // default constructor
    explicit Type(TypeKind k) : fundamental_kind(k) {}         // kind-only constructor
    
    // --------------------------------------------
    // For When Checking Type We Can Query Properly
    // --------------------------------------------
    // ---------------------- Type Queries ----------------------

    inline bool is_primitive(const std::shared_ptr<Type> t) {
        if (!t) return false;
        switch (t->fundamental_kind) {
            case TypeKind::Int:
            case TypeKind::Bool:
            case TypeKind::Char:
            case TypeKind::Real:
            case TypeKind::String:
                return true;
            default:
                return false;
        }
    }


    inline bool is_array(const std::shared_ptr<Type> t) {
        return t && t->fundamental_kind == TypeKind::Array && array_type_prim && array_size >= 0;
    }

    inline bool is_slice(const std::shared_ptr<Type> t) {
        return t && t->fundamental_kind == TypeKind::Slice && array_type_prim && array_size == -1;
    }

    inline bool is_tuple(const std::shared_ptr<Type> t) {
        return t && t->fundamental_kind == TypeKind::Tuple && !tuple_types.empty();
    }

    inline bool is_named(const std::shared_ptr<Type>t) {
        return t && t->fundamental_kind == TypeKind::Named && !alias.empty();
    }

    inline bool is_void(const std::shared_ptr<Type> t) {
        return t && t->fundamental_kind == TypeKind::Void;
    }

    inline bool is_error(const std::shared_ptr<Type> t) {
        return !t || t->fundamental_kind == TypeKind::Error;
    }


    // ----------------------
    // Debug / printing
    // ----------------------
    std::string to_string() const {
        switch (fundamental_kind) {
            case TypeKind::Int:    return "Int";
            case TypeKind::Bool:   return "Bool";
            case TypeKind::Char:   return "Char";
            case TypeKind::Real:   return "Real";
            case TypeKind::String: return "String";
            case TypeKind::Void:   return "Void";
            case TypeKind::Range:  return "Range";
            case TypeKind::Error:  return "<error>";
            case TypeKind::Function: return "Function";

            case TypeKind::Slice:  return array_type_prim->to_string() + "[]";
            case TypeKind::Array:  return array_type_prim->to_string() + "[" + std::to_string(array_size) + "]";
            case TypeKind::Tuple: {
                std::string s = "(";
                for (size_t i = 0; i < tuple_types.size(); ++i) {
                    if (i) s += ", ";
                    s += tuple_types[i]->to_string();
                }
                s += ")";
                return s;
            }
            case TypeKind::Named: return alias;
        }
        return "<?>";
    }
};

using type_ptr = std::shared_ptr<Type>;

} // namespace rex