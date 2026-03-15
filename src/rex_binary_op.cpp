#include "rex_binary_op.h"
#include "rex_types.h"
#include <iostream>
#include <algorithm>
#include <memory>

using namespace rex;
using PrimKind = PrimType::Prims;

// -------------------------------------------------
// Type helpers
// -------------------------------------------------

template<typename T>
inline std::shared_ptr<T> as(const type_ptr& t) {
    return std::static_pointer_cast<T>(t);
}

inline bool BinaryOpSystem::is_prim(type_ptr t, PrimKind k) {
    return t->kind == TypeKind::Primitive &&
           as<PrimType>(t)->prim == k;
}

// -------------------------------------------------
// Type checks
// -------------------------------------------------

bool BinaryOpSystem::is_primitive(type_ptr T) {
    return T->kind == TypeKind::Primitive;
}

bool BinaryOpSystem::is_array(type_ptr T) {
    return T->kind == TypeKind::Array;
}

bool BinaryOpSystem::is_tuple(type_ptr T) {
    return T->kind == TypeKind::Tuple;
}

bool BinaryOpSystem::is_slice(type_ptr T) {
    return T->kind == TypeKind::Slice;
}

bool BinaryOpSystem::is_func(type_ptr T) {
    return T->kind == TypeKind::Function;
}

bool BinaryOpSystem::is_numeric(type_ptr T) {
    if (!is_primitive(T)) return false;

    auto p = as<PrimType>(T);

    return p->prim == PrimKind::Int ||
           p->prim == PrimKind::Real ||
           p->prim == PrimKind::Char;
}

bool BinaryOpSystem::is_bool(type_ptr T) {
    return is_prim(T, PrimKind::Bool);
}

bool BinaryOpSystem::is_integer(type_ptr T) {
    return is_prim(T, PrimKind::Int);
}

bool BinaryOpSystem::is_real(type_ptr T) {
    return is_prim(T, PrimKind::Real);
}

bool BinaryOpSystem::is_string(type_ptr T) {
    return is_prim(T, PrimKind::String);
}

bool BinaryOpSystem::is_char(type_ptr T) {
    return is_prim(T, PrimKind::Char);
}

// -------------------------------------------------
// Helpers: operation categories
// -------------------------------------------------
bool BinaryOpSystem::is_logic(BinaryOp op){
    return op == BinaryOp::AND || op == BinaryOp::OR;
}

bool BinaryOpSystem::is_comp(BinaryOp op){
    switch(op){
        case BinaryOp::LT: case BinaryOp::GT: case BinaryOp::LE: case BinaryOp::GE: 
        case BinaryOp::EQ: case BinaryOp::NEQ:
            return true;
        default:
            return false;
    }
}

bool BinaryOpSystem::is_arth(BinaryOp op){
    switch(op){
        case BinaryOp::ADD: case BinaryOp::SUB: case BinaryOp::MUL: 
        case BinaryOp::DIV: case BinaryOp::MOD:
            return true;
        default: return false;
    }
}

// -------------------------------------------------
// Primitive ranking & promotion
// -------------------------------------------------
int BinaryOpSystem::prim_rank(PrimKind k){
    switch(k){
        case PrimKind::Bool: return 0;
        case PrimKind::Char: return 1;
        case PrimKind::Int:  return 2;
        case PrimKind::Real: return 3;
        default: return -1;
    }
}

type_ptr BinaryOpSystem::promote_primitive(std::shared_ptr<PrimType> A, std::shared_ptr<PrimType> B){
    return std::make_shared<PrimType>(prim_rank(A->prim) >= prim_rank(B->prim) ? A->prim : B->prim);
}

// -------------------------------------------------
// General promotion
// -------------------------------------------------
type_ptr BinaryOpSystem::promote(type_ptr L, type_ptr R, const std::string& op){


    // same type
    if(L->equals(R)) {
        if((is_prim(L, PrimKind::String) && op != "+"))
            throw std::runtime_error("Cannot apply operator '" + op + "' to String and String");
        
        if(is_prim(L, PrimKind::Char) && op != "+") 
            throw std::runtime_error("Cannot apply operator '" + op + "' to Char and Char");
        
        if(is_prim(L, PrimKind::Char) && op == "+"){
            return std::make_shared<PrimType>(PrimKind::String);
        }
                    
        return L;
    }

    // primitive + primitive
    if(is_primitive(L) && is_primitive(R)){
        auto prim_l = as<PrimType>(L);
        auto prim_r = as<PrimType>(R);

        if (is_string(L) || is_string(R)) {
            if (op != "+" || !((is_string(L) || is_char(L)) &&
                            (is_string(R) || is_char(R))))
                throw std::runtime_error(
                    "Cannot apply dasd '" + op + "' to " +
                    L->to_string() + " and " + R->to_string()
                );
            return std::make_shared<PrimType>(PrimKind::String);
        }


        return promote_primitive(prim_l, prim_r);
    }
        

    // array + primitive
    if(is_array(L) && is_numeric(R)){
        auto arr = std::static_pointer_cast<ArrayType>(L);
        auto elem = promote(arr->elem, R, op);
        return std::make_shared<ArrayType>(elem, arr->size);
    }
    if(is_array(R) && is_numeric(L)){
        auto arr = std::static_pointer_cast<ArrayType>(R);
        auto elem = promote(arr->elem, L, op);
        return std::make_shared<ArrayType>(elem, arr->size);
    }

    // array + array
    if(is_array(L) && is_array(R)){
        auto a1 = std::static_pointer_cast<ArrayType>(L);
        auto a2 = std::static_pointer_cast<ArrayType>(R);
        auto elem = promote(a1->elem, a2->elem, op);
        return std::make_shared<ArrayType>(elem, std::max(a1->size, a2->size));
    }

    // tuple + tuple
    if(is_tuple(L) && is_tuple(R)){
        auto t1 = std::static_pointer_cast<TupleType>(L);
        auto t2 = std::static_pointer_cast<TupleType>(R);
        if(t1->elements.size() != t2->elements.size())
            throw std::runtime_error("Tuple size mismatch in op " + op);

        std::vector<type_ptr> elems(t1->elements.size());
        for(size_t i=0;i<elems.size();i++)
            elems[i] = promote(t1->elements[i], t2->elements[i], op);
        return std::make_shared<TupleType>(elems);
    }

    // func + func
    if(is_func(L) && is_func(R)){
        auto f1 = std::static_pointer_cast<FunctionType>(L);
        auto f2 = std::static_pointer_cast<FunctionType>(R);
        return promote(f1->ret, f2->ret, op);
    }

    // func + primitive/array
    if(is_func(L) && (is_primitive(R) || is_array(R)))
        return promote(as<FunctionType>(L)->ret, R, op);
    if(is_func(R) && (is_primitive(L) || is_array(L)))
        return promote(as<FunctionType>(R)->ret, L, op);

    throw std::runtime_error("Cannot apply operator '" + op + "' to " + L->to_string() + " and " + R->to_string());
}

// -------------------------------------------------
// Unary
// -------------------------------------------------
type_ptr BinaryOpSystem::check_unary(UniOp op, type_ptr operand){
    switch(op){
        case UniOp::POS:
        case UniOp::NEG:
            if(!is_numeric(operand))
                throw std::runtime_error("Unary +/- requires numeric type");
            return operand;
        default:
            throw std::runtime_error("Unknown unary op");
    }
}

// -------------------------------------------------
// Range
// -------------------------------------------------
type_ptr BinaryOpSystem::check_range(type_ptr L, type_ptr R){
    if(!is_numeric(L) || !is_numeric(R))
        throw std::runtime_error("Range operator requires numeric types");
    if(!is_integer(L) || !is_integer(R))
        throw std::runtime_error("Range operator requires integer types");

    return std::make_shared<RangeType>(L,R);
}

// -------------------------------------------------
// Pipe
// -------------------------------------------------
type_ptr BinaryOpSystem::check_pipe(type_ptr value, type_ptr fnType){
    auto fn = std::static_pointer_cast<FunctionType>(fnType);
    return fn->ret;
}

// -------------------------------------------------
// Indexing
// -------------------------------------------------
type_ptr BinaryOpSystem::check_index(type_ptr base, type_ptr index){
    if(!is_array(base))
        throw std::runtime_error("Cannot index non-array type");
    if(!is_integer(index))
        throw std::runtime_error("Array index must be integer");

    if(is_array(base))
        return std::static_pointer_cast<ArrayType>(base)->elem;
    if(is_slice(base))
        return std::static_pointer_cast<SliceType>(base)->elem;

    throw std::runtime_error("Unknown array-like type");
}

// -------------------------------------------------
// Binary operators
// -------------------------------------------------
type_ptr BinaryOpSystem::check_binary(BinaryOp op, type_ptr L, type_ptr R){
    if(debug)
        std::cout << "L=" << L->to_fundamental_string() << " R=" << R->to_fundamental_string() << "\n";

    if(is_arth(op)) {return promote(L,R,binop_name(op));}
    if(is_comp(op)){
        std::cout << "hello world" << std::endl;
        auto re = promote(L,R,binop_name(op));
        if(is_array(L) || is_array(R))
            std::static_pointer_cast<ArrayType>(re)->elem = std::make_shared<PrimType>(PrimKind::Bool);
        return std::make_shared<PrimType>(PrimKind::Bool);
    }
    if(is_logic(op)){
        if(!is_bool(L) || !is_bool(R))
            throw std::runtime_error("and/or require Bool operands");
        return promote(L,R,binop_name(op));
    }
    switch(op){
        case BinaryOp::RANGE: return check_range(L,R);
        case BinaryOp::PIPE:  return check_pipe(L,R);
        default: throw std::runtime_error("Unknown binary operator");
    }
}