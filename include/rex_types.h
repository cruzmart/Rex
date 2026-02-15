#pragma once

#include <memory>
#include <string>
#include <vector>

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
    Error    // type-checking failure
};

// -------------------------------------------------
// Type representation
// -------------------------------------------------

struct type {
    type_kind kind;

    // For Array / Slice
    std::shared_ptr<type> element; // element type
    int array_size = -1;           // >=0 only for Array

    // For Tuple
    std::vector<type> elements;

     // Default constructor
    type() : kind(type_kind::Error) {}

 

    // -------------------------------------------------
    // Constructors
    // -------------------------------------------------

    explicit type(type_kind k) : kind(k) {}

    static type Int()    { return type(type_kind::Int); }
    static type Bool()   { return type(type_kind::Bool); }
    static type Char()   { return type(type_kind::Char); }
    static type Real()   { return type(type_kind::Real); }
    static type String() { return type(type_kind::String); }

    static type Range()  { return type(type_kind::Range); }
    static type Void()   { return type(type_kind::Void); }
    static type Error()  { return type(type_kind::Error); }

    static type Slice(type elem) {
        type t(type_kind::Slice);
        t.element = std::make_shared<type>(elem);
        return t;
    }

    static type Array(type elem, int size) {
        type t(type_kind::Array);
        t.element = std::make_shared<type>(elem);
        t.array_size = size;
        return t;
    }

    static type Tuple(std::vector<type> elems) {
        type t(type_kind::Tuple);
        t.elements = std::move(elems);
        return t;
    }

    // -------------------------------------------------
    // Classification helpers
    // -------------------------------------------------

    bool is_numeric() const {
        return kind == type_kind::Char ||
               kind == type_kind::Int  ||
               kind == type_kind::Real;
    }

    bool is_integral() const {
        return kind == type_kind::Char ||
               kind == type_kind::Int;
    }

    bool is_indexable() const {
        return kind == type_kind::Array ||
               kind == type_kind::Slice ||
               kind == type_kind::Tuple;
    }

    bool is_container() const {
        return kind == type_kind::Array ||
               kind == type_kind::Slice;
    }

    // -------------------------------------------------
    // Equality
    // -------------------------------------------------

    bool operator==(const type& other) const {
        if (kind != other.kind) return false;

        switch (kind) {
            case type_kind::Array:
                return array_size == other.array_size &&
                       *element == *other.element;

            case type_kind::Slice:
                return *element == *other.element;

            case type_kind::Tuple:
                return elements == other.elements;

            default:
                return true;
        }
    }

    bool operator!=(const type& other) const {
        return !(*this == other);
    }

    // -------------------------------------------------
    // Debug / printing
    // -------------------------------------------------

    std::string to_string() const {
        switch (kind) {
            case type_kind::Int:    return "Int";
            case type_kind::Bool:   return "Bool";
            case type_kind::Char:   return "Char";
            case type_kind::Real:   return "Real";
            case type_kind::String: return "String";

            case type_kind::Range:  return "Range";
            case type_kind::Void:   return "Void";
            case type_kind::Error:  return "<error>";

            case type_kind::Slice:
                return element->to_string() + "[]";

            case type_kind::Array:
                return element->to_string() +
                       "[" + std::to_string(array_size) + "]";

            case type_kind::Tuple: {
                std::string s = "(";
                for (size_t i = 0; i < elements.size(); ++i) {
                    if (i) s += ", ";
                    s += elements[i].to_string();
                }
                s += ")";
                return s;
            }
        }
        return "<?>";
    }
};

} // namespace rex
