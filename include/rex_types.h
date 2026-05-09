#pragma once
#include "rex_ast.h"
#include "rex_funcs.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace rex {

// -------------------------------------------------
// Forward declarations
// -------------------------------------------------
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
    Range,
    Pipe,
    Array,
    Slice,
    Tuple,
    Id,
    Void,
    Error,
    Named,
    Function
};

// -------------------------------------------------
// Base type
// -------------------------------------------------
struct Type  {
    TypeKind kind;
    SourceLocation loc;
    
    Type() : kind(TypeKind::Error) {}
    explicit Type(TypeKind k) : kind(k) {}
    virtual ~Type() = default;

    virtual bool equals(const std::shared_ptr<Type> other) const {
        return kind == other->kind;
    }

    virtual std::string to_string() const {
        return "<type>";
    }

    std::string to_fundamental_string() const {
        switch (kind) {
            case TypeKind::Id:       return "Id";
            case TypeKind::Void:     return "Void";
            case TypeKind::Range:    return "Range";
            case TypeKind::Pipe:     return "Pipe";
            case TypeKind::Array:    return "Array";
            case TypeKind::Slice:    return "Slice";
            case TypeKind::Tuple:    return "Tuple";
            case TypeKind::Named:    return "Named";
            case TypeKind::Function: return "Function";
            case TypeKind::Primitive:return "Primitive";
            case TypeKind::Error:    return "<error>";
        }
        return "<?>";
    }

};

// -------------------------------------------------
// Primitive type
// -------------------------------------------------
struct PrimType : Type {
    enum class Prims { Int, Bool, Char, Real, String, Null, Void} prim;

    explicit PrimType(Prims p) : Type(TypeKind::Primitive), prim(p) {}
    
    std::string to_string() const override {
        switch(prim){
            case Prims::Int:    return "Int";
            case Prims::Bool:   return "Bool";
            case Prims::Char:   return "Char";
            case Prims::Real:   return "Real";
            case Prims::String: return "String";
            case Prims::Null:   return "Null";
            case Prims::Void:   return "Void";
        }
        return "<?>";
    }

    bool equals(const std::shared_ptr<Type> other) const override {
        if(kind != other->kind) return false;
        auto o = std::static_pointer_cast<PrimType>(other);
        return prim == o->prim;
    }
};

// -------------------------------------------------
// Named type (alias)
// -------------------------------------------------
struct NamedType : Type {
    std::string alias;
    std::shared_ptr<Type> actual;

    NamedType(std::string name, std::shared_ptr<Type> real = nullptr)
        : Type(TypeKind::Named), alias(std::move(name)), actual(std::move(real)) {}

    std::string to_string() const override {
        if(actual) return alias + " -> " + actual->to_string();
        return alias;
    }

    bool equals(const std::shared_ptr<Type> other) const override {
        if(kind != other->kind) return false;
        auto o = std::static_pointer_cast<NamedType>(other);
        if(!actual && !o->actual) return true;
        if(!actual || !o->actual) return false;
        return actual->equals(o->actual);
    }
};

// -------------------------------------------------
// Array / Slice types
// -------------------------------------------------

enum class ArrayStorageKind {
    GlobalConst,
    RuntimeAlloc
};

struct ArrayType : Type {
    std::shared_ptr<Type> elem;
    int size = 0;

    ArrayStorageKind arrayKind;

    explicit ArrayType(std::shared_ptr<Type> e, int s = 0)
        : Type(TypeKind::Array), elem(std::move(e)), size(s) {}

    std::string to_string() const override {
        if(elem->kind == TypeKind::Array){
            auto elem_arr = std::static_pointer_cast<ArrayType>(elem);
            return elem_arr->elem->to_string() + "[" + std::to_string(size) + "]" + "[" + std::to_string(elem_arr->size) +"]";
        }
        return elem->to_string() + "[" + std::to_string(size) + "]";
    }

    bool equals(const std::shared_ptr<Type> other) const override {
        // Broke For Now.
        if(kind != other->kind) return false;
        auto o = std::static_pointer_cast<ArrayType>(other);
        return size == o->size && elem->equals(o->elem);
    }

    PrimType::Prims matrixType(){
        if(isMatrix())
            return std::static_pointer_cast<PrimType>(std::static_pointer_cast<ArrayType>(elem)->elem)->prim;
        return PrimType::Prims::Null;
    }

    PrimType::Prims arrayType() {
        if(isArray())
            return std::static_pointer_cast<PrimType>(elem)->prim;
        return PrimType::Prims::Null;
    }
    
    bool isMatrix(){
        return elem->kind == TypeKind::Array;
    }

    bool isArray(){
        return isMatrix() != true;
    }

    std::pair<int,int> dimensions(){
     
        if (isMatrix()) {
            auto arrElem = std::static_pointer_cast<ArrayType>(elem);
            return {size, arrElem->size};
        }

        return {1, size};
    }

};

struct SliceType : Type {
    std::shared_ptr<Type> elem;

    explicit SliceType(std::shared_ptr<Type> e) : Type(TypeKind::Slice), elem(std::move(e)) {}

    std::string to_string() const override {
        return elem->to_string() + "[]";
    }

    bool equals(const std::shared_ptr<Type> other) const override {
        if(kind != other->kind) return false;
        auto o = std::static_pointer_cast<SliceType>(other);
        return elem->equals(o->elem);
    }
};

// -------------------------------------------------
// Tuple type
// -------------------------------------------------
struct TupleType : Type {
    std::vector<std::shared_ptr<Type>> elements;

    explicit TupleType(std::vector<std::shared_ptr<Type>> elems)
        : Type(TypeKind::Tuple), elements(std::move(elems)) {}
    explicit TupleType() : Type(TypeKind::Tuple) {}

    std::string to_string() const override {
        std::string s = "(";
        for(size_t i=0;i<elements.size();++i){
            s += elements[i]->to_string();
            if(i+1 < elements.size()) s+=", ";
        }
        s += ")";
        return s;
    }

    bool equals(const std::shared_ptr<Type> other) const override {
        if(kind != other->kind) return false;
        auto o = std::static_pointer_cast<TupleType>(other);
        if(elements.size() != o->elements.size()) return false;
        for(size_t i=0;i<elements.size();++i){
            if(!elements[i]->equals(o->elements[i])) return false;
        }
        return true;
    }
};

// -------------------------------------------------
// Range type
// -------------------------------------------------
struct RangeType : Type {
    std::shared_ptr<Type> lower, upper;

    RangeType(std::shared_ptr<Type> l, std::shared_ptr<Type> u)
        : Type(TypeKind::Range), lower(std::move(l)), upper(std::move(u)) {}

    std::string to_string() const override {
        return "Range(" + lower->to_string() + " .. " + upper->to_string() + ")";
    }

    bool equals(const std::shared_ptr<Type> other) const override {
        if(kind != other->kind) return false;
        auto o = std::static_pointer_cast<RangeType>(other);
        return lower->equals(o->lower) && upper->equals(o->upper);
    }
};

// -------------------------------------------------
// Pipe type
// -------------------------------------------------
struct PipeType : Type {
    std::shared_ptr<Type> left, right;

    PipeType(std::shared_ptr<Type> l, std::shared_ptr<Type> r)
        : Type(TypeKind::Pipe), left(std::move(l)), right(std::move(r)) {}

    std::string to_string() const override {
        return left->to_string() + " |> " + right->to_string();
    }

    bool equals(const std::shared_ptr<Type> other) const override {
        if(kind != other->kind) return false;
        auto o = std::static_pointer_cast<PipeType>(other);
        return left->equals(o->left) && right->equals(o->right);
    }
};

// -------------------------------------------------
// Function type
// -------------------------------------------------
struct FunctionType : Type {
    std::vector<std::shared_ptr<Parameter>> params;
    std::shared_ptr<Type> ret;

    FunctionType(std::vector<std::shared_ptr<Parameter>> p, std::shared_ptr<Type> r)
        : Type(TypeKind::Function), params(std::move(p)), ret(std::move(r)) {}

    FunctionType() : Type(TypeKind::Function) {}

    std::string to_string() const override {
        std::string s = "(";
        for(size_t i=0;i<params.size();++i){
            s += params[i]->para_name + " : " + params[i]->para_type->to_string();
            if(i+1<params.size()) s+=", ";
        }
        s += ") -> " + ret->to_string();
        return s;
    }

    bool equals(const std::shared_ptr<Type> other) const override {
        if(kind != other->kind) return false;
        auto o = std::static_pointer_cast<FunctionType>(other);
        if(params.size() != o->params.size()) return false;
        for(size_t i=0;i<params.size();++i){
            if(!params[i]->para_type->equals(o->params[i]->para_type)) return false;
        }
        return ret->equals(o->ret);
    }
};

// -------------------------------------------------
// Shared pointer aliases
// -------------------------------------------------
using type_ptr  = std::shared_ptr<Type>;
using prim_ptr  = std::shared_ptr<PrimType>;
using named_ptr = std::shared_ptr<NamedType>;
using array_ptr = std::shared_ptr<ArrayType>;
using slice_ptr = std::shared_ptr<SliceType>;
using tuple_ptr = std::shared_ptr<TupleType>;
using range_ptr = std::shared_ptr<RangeType>;
using pipe_ptr  = std::shared_ptr<PipeType>;
using func_ptr  = std::shared_ptr<FunctionType>;

// -------------------------------------------------
// Type converter helper
// -------------------------------------------------
template<typename T>
inline std::shared_ptr<T> as_type(const std::shared_ptr<Type>& t) {
    return std::static_pointer_cast<T>(t);
}

} // namespace rex