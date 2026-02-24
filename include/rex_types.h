#pragma once

#include <memory>
#include <string>
#include <vector>
#include <iostream>

namespace rex {

// -------------------------------------------------
// Type kinds supported by Rex
// -------------------------------------------------
enum class type_kind {
    Int,
    Bool,
    Char,
    Real,
    String,

    Array,   // fixed-size array: T[n]
    Slice,   // dynamic-size slice: T[]
    Tuple,   // tuple: (T1, T2, ...)

    Range,   // result of a..b
    Void,    // statements / no value
    Error,    // type-checking failure

    Named,      //for type aliasing 

    Function

};

// -------------------------------------------------
// Type representation
// -------------------------------------------------
struct Type {
    type_kind kind;

    // type alias
    std::string alias;

    // For Array / Slice
    std::shared_ptr<Type> element; // element type
    int array_size = -1;           // >=0 only for Array

    // For Tuple
    std::vector<std::shared_ptr<Type>> elements;

    // ----------------------
    // Constructors
    // ----------------------
    Type() : kind(type_kind::Error) {}               // default constructor
    explicit Type(type_kind k) : kind(k) {}         // kind-only constructor

    // Copy constructor
    Type(const Type& other)
        : kind(other.kind),
          element(other.element ? std::make_shared<Type>(*other.element) : nullptr),
          array_size(other.array_size)
    {
        elements.reserve(other.elements.size());
        for (auto& e : other.elements) {
            elements.push_back(e ? std::make_shared<Type>(*e) : nullptr);
        }
    }

    // Move constructor
    Type(Type&& other) noexcept
        : kind(other.kind),
          element(std::move(other.element)),
          array_size(other.array_size),
          elements(std::move(other.elements))
    {}

    // Copy assignment
    Type& operator=(const Type& other) {
        if (this != &other) {
            kind = other.kind;
            element = other.element ? std::make_shared<Type>(*other.element) : nullptr;
            array_size = other.array_size;

            elements.clear();
            elements.reserve(other.elements.size());
            for (auto& e : other.elements) {
                elements.push_back(e ? std::make_shared<Type>(*e) : nullptr);
            }
        }
        return *this;
    }

    // Move assignment
    Type& operator=(Type&& other) noexcept {
        if (this != &other) {
            kind = other.kind;
            element = std::move(other.element);
            array_size = other.array_size;
            elements = std::move(other.elements);
        }
        return *this;
    }

    // ----------------------
    // Factory methods
    // ----------------------
    
    static std::shared_ptr<Type> Int() {
        return std::make_shared<Type>(type_kind::Int);
    }

    static std::shared_ptr<Type> Bool() {
        return std::make_shared<Type>(type_kind::Bool);
    }

    static std::shared_ptr<Type> Char() {
        return std::make_shared<Type>(type_kind::Char);
    }

    static std::shared_ptr<Type> Real() {
        return std::make_shared<Type>(type_kind::Real);
    }

    static std::shared_ptr<Type> String() {
        return std::make_shared<Type>(type_kind::String);
    }

    static std::shared_ptr<Type> Void() {
        return std::make_shared<Type>(type_kind::Void);
    }

    static std::shared_ptr<Type> Range() {
        return std::make_shared<Type>(type_kind::Range);
    }

    static std::shared_ptr<Type> Error() {
        return std::make_shared<Type>(type_kind::Error);
    }

    // Composite types
    static std::shared_ptr<Type> Slice(const std::shared_ptr<Type>& elem) {
        auto t = std::make_shared<Type>(type_kind::Slice);
        t->element = elem;
        return t;
    }

    static std::shared_ptr<Type> Array(const std::shared_ptr<Type>& elem, int size) {
        auto t = std::make_shared<Type>(type_kind::Array);
        t->element = elem;
        t->array_size = size;
        return t;
    }

    static std::shared_ptr<Type> Tuple(const std::vector<std::shared_ptr<Type>>& elems) {
        auto t = std::make_shared<Type>(type_kind::Tuple);
        t->elements = elems; // already shared_ptrs, just copy
        return t;
    }

    static std::shared_ptr<Type> Named(const std::string name){
        auto t = std::make_shared<Type>(type_kind::Named);
        t->alias = name;
        return t;
    }
    // ----------------------
    // Equality operators
    // ----------------------
    bool operator==(const Type& other) const {
        if (kind != other.kind) return false;

        switch (kind) {
            case type_kind::Array:
                return array_size == other.array_size &&
                       *element == *other.element;

            case type_kind::Slice:
                return *element == *other.element;

            case type_kind::Tuple:
                if (elements.size() != other.elements.size()) return false;
                for (size_t i = 0; i < elements.size(); ++i) {
                    if (*elements[i] != *other.elements[i]) return false;
                }
                return true;

            default:
                return true;
        }
    }

    bool operator!=(const Type& other) const { return !(*this == other); }

    // ----------------------
    // Debug / printing
    // ----------------------
    std::string to_string() const {
        switch (kind) {
            case type_kind::Int:    return "Int";
            case type_kind::Bool:   return "Bool";
            case type_kind::Char:   return "Char";
            case type_kind::Real:   return "Real";
            case type_kind::String: return "String";
            case type_kind::Void:   return "Void";
            case type_kind::Range:  return "Range";
            case type_kind::Error:  return "<error>";
            case type_kind::Function: return "Function";

            case type_kind::Slice:  return element->to_string() + "[]";
            case type_kind::Array:  return element->to_string() + "[" + std::to_string(array_size) + "]";
            case type_kind::Tuple: {
                std::string s = "(";
                for (size_t i = 0; i < elements.size(); ++i) {
                    if (i) s += ", ";
                    s += elements[i]->to_string();
                }
                s += ")";
                return s;
            }
            case type_kind::Named: return alias;
        }
        return "<?>";
    }
};

using type_ptr = std::shared_ptr<Type>;

} // namespace rex