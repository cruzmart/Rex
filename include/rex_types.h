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
    Pipe,    // f |> g
    Array,   // fixed-size array: T[n]
    Slice,   // dynamic-size slice: T[]
    Tuple,   // tuple: (T1, T2, ...)

    Id,


    Void,     // statements / no value
    Error,    // type-checking failure

    Named,      //for type aliasing 

    Function

};

// -------------------------------------------------
// Type representation
// -------------------------------------------------
struct Type {

    TypeKind fundamental_kind;

    // ----------------------
    // Constructors
    // ----------------------
    Type() : fundamental_kind(TypeKind::Error) {}               // default constructor
    explicit Type(TypeKind k) : fundamental_kind(k) {}         // kind-only constructor
    virtual ~Type() = default;  // <--- makes Type polymorphic

    // ----------------------
    // Debug / printing
    // ----------------------
    std::string to_string() const {
        switch (fundamental_kind) {
            case TypeKind::Id:    return "Id";
            case TypeKind::Void:   return "Void";
            case TypeKind::Range:  return "Range";
            case TypeKind::Error:  return "<error>";
            case TypeKind::Function: return "Function";
            case TypeKind::Slice:  return "Slice";
            case TypeKind::Array:  return "Array";
            case TypeKind::Tuple:  return "Tuple";
            case TypeKind::Named: return "Named";
            case TypeKind::Pipe: return "Pipe";
            case TypeKind::Primitive: return "Primitive";
        }
        return "<?>";
    }
};

using type_ptr = std::shared_ptr<Type>;

} // namespace rex