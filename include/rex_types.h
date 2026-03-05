#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>



namespace rex {

struct PrimType;
struct NamedType;
struct ArrayType;
struct SliceType;
struct TupleType;
struct RangeType;
struct PipeType;
struct FunctionType;


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
    std::string to_fundamental_string() const {
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

      // NEW
    virtual bool equals(const std::shared_ptr<Type> other) const {
        return fundamental_kind == other->fundamental_kind;
    }

    virtual std::string to_string() const {
        return "<type>";
    }
};


// ---------------------- TYPES --------------------------

struct NamedType : Type {
    // type alias
    std::string alias;
    std::shared_ptr<Type> actual_type;

    NamedType() : Type(TypeKind::Named) {}
    NamedType(std::string name) : Type(TypeKind::Named), alias(name){}
    NamedType(std::string name, std::shared_ptr<Type> real_type) : Type(TypeKind::Named), alias(name), actual_type(real_type) {} 

    std::string to_string() const override {
        if (actual_type)
            return alias + " -> " + actual_type->to_string();
        return alias;
    }
};

struct ArrayType : Type {

    // For Array / Slice
    std::shared_ptr<Type> array_type; 
    int size = -1;   

    ArrayType() : Type(TypeKind::Array), array_type(std::make_shared<Type>()){}       
    ArrayType(std::shared_ptr<Type> elem, int sz) : Type(TypeKind::Array), array_type(elem), size(sz) {}

    bool equals(const std::shared_ptr<Type> other) const override {
        if (fundamental_kind != other->fundamental_kind) return false;

        auto o = std::dynamic_pointer_cast<ArrayType>(other);
        if (!o) return false;

        return array_type->equals(o->array_type);
    }
    
    std::string to_string() const override {
        return array_type->to_string() + "[" + std::to_string(size) + "]";
    }

};

struct SliceType : Type {
     // For Array / Slice
    std::shared_ptr<Type> slice_type;

    SliceType() : Type(TypeKind::Slice), slice_type(std::make_shared<Type>()) {}       
    SliceType(std::shared_ptr<Type> elem) : Type(TypeKind::Slice), slice_type(elem) {}

    bool equals(const std::shared_ptr<Type> other) const override {
        if (fundamental_kind != other->fundamental_kind) return false;

        auto o = std::dynamic_pointer_cast<SliceType>(other);
        if (!o) return false;

        return slice_type->equals(o->slice_type);
    }

    std::string to_string() const override {
        return slice_type->to_string() + "[]";
    }
};

struct RangeType : Type {
    std::shared_ptr<Type> lower_type;
    std::shared_ptr<Type> upper_type;

    RangeType() : Type(TypeKind::Range), lower_type(std::make_shared<Type>()), upper_type(std::make_shared<Type>()) {}
    RangeType(std::shared_ptr<Type> l, std::shared_ptr<Type> u) : Type(TypeKind::Range), lower_type(l), upper_type(u) {}

    std::string to_string() const override {
        return "Range(" + lower_type->to_string() +
               " .. " +
               upper_type->to_string() + ")";
    }
};

struct PipeType : Type {
    std::shared_ptr<Type> func_left_type;
    std::shared_ptr<Type> func_right_type;

    PipeType() : Type(TypeKind::Pipe) {}
    PipeType(std::shared_ptr<Type> l, std::shared_ptr<Type> u) : Type(TypeKind::Pipe),func_left_type(l), func_right_type(u) {}
};

struct TupleType : Type {

     // For Tuple
    std::vector<std::shared_ptr<Type>> tuple_types;

    TupleType() : Type(TypeKind::Tuple) {}

    std::string to_string() const override {
        std::string out = "(";
        for(size_t i = 0; i < tuple_types.size(); ++i){
            out += tuple_types[i]->to_string();
            if(i + 1  < tuple_types.size()) out += ", ";
        }
        out += ")";

        return out;
    }

};
struct PrimType  : Type {
    enum class Prims { 
    Int,
    Bool,
    Char,
    Real,
    String};

    Prims prim_type;

    PrimType(Prims d) : Type(TypeKind::Primitive), prim_type(d) {}
    PrimType() : Type(TypeKind::Primitive) {}

    std::string to_string() const override {
        switch(prim_type){
            case Prims::Int: return "Int";
            case Prims::Bool: return "Bool";
            case Prims::Char: return "Char";
            case Prims::Real: return "Real";
            case Prims::String: return "String";
            default:
                return "<?>";
        }
    }

};

struct FunctionType : Type {
    std::vector<std::shared_ptr<Type>> params_type;
    std::shared_ptr<Type> return_type;

    FunctionType() : Type(TypeKind::Function) {}

    FunctionType(std::vector<std::shared_ptr<Type>> params, std::shared_ptr<Type> ret)
        : Type(TypeKind::Function), params_type(std::move(params)), return_type(ret) {}

    std::string to_string() const override {
        std::string s = "(";
        for (size_t i=0; i<params_type.size(); ++i) {
            s += params_type[i]->to_string();
            if (i+1 < params_type.size()) s += ", ";
        }
        s += ") -> " + return_type->to_string();
        return s;
    }
};

using type_ptr = std::shared_ptr<Type>;

} // namespace rex