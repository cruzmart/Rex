#include "rex_operator_type_system.h"
#include "rex_ast_nodes.h"
#include "rex_ops.h"
#include "rex_types.h"
#include <memory>
#include <stdexcept>
#include <iostream>

using namespace rex;
using PrimKind = rex::PrimType::Prims;

//
// BASIC HELPERS
//


// HELPERS TO FIND WHAT PRIM TYPE IT IS

bool OperatorTypeSystem::is_logic(BinaryOp op){
    switch(op){
    case BinaryOp::AND:
    case BinaryOp::OR:
        return true;
    default:
        return false;
    }
}
bool OperatorTypeSystem::is_comp(BinaryOp op){
    switch(op){
    case BinaryOp::LT:
    case BinaryOp::GT:
    case BinaryOp::LE:
    case BinaryOp::GE:
    case BinaryOp::EQ:
    case BinaryOp::NEQ:
        return true;
    default:
        return false;
    }

}
bool OperatorTypeSystem::is_arth(BinaryOp op){
    switch (op) {
        case BinaryOp::ADD:
        case BinaryOp::SUB:
        case BinaryOp::MUL:
        case BinaryOp::DIV:
        case BinaryOp::MOD:
            return true;
        default:
            return false;
    }
}

bool OperatorTypeSystem::is_primitive(type_ptr T) {
    return std::dynamic_pointer_cast<PrimType>(T) != nullptr;
}

bool OperatorTypeSystem::is_integer(type_ptr T){
    if(!is_primitive(T)) return false;
    if(auto p = std::dynamic_pointer_cast<PrimType>(T))
        return p->prim_type == PrimKind::Int;
    return false;
}
bool OperatorTypeSystem::is_real(type_ptr T){
    if(!is_primitive(T)) return false;
    if(auto p = std::dynamic_pointer_cast<PrimType>(T))
        return p->prim_type == PrimKind::Real;
    return false;

}
bool OperatorTypeSystem::is_string(type_ptr T){
    if(!is_primitive(T)) return false;
    if(auto p = std::dynamic_pointer_cast<PrimType>(T))
        return p->prim_type == PrimKind::String;
    return false;
}
bool OperatorTypeSystem::is_char(type_ptr T){
    if(!is_primitive(T)) return false;
    if(auto p = std::dynamic_pointer_cast<PrimType>(T))
        return p->prim_type == PrimKind::Char;
    return false;
}
bool OperatorTypeSystem::is_bool(type_ptr T) {
    if(!is_primitive(T)) return false;
    if (auto p = std::dynamic_pointer_cast<PrimType>(T))
        return p->prim_type == PrimKind::Bool;
    return false;
}

bool OperatorTypeSystem::is_numeric(type_ptr T) {
    if (auto p = std::dynamic_pointer_cast<PrimType>(T)) {
        return p->prim_type == PrimKind::Int ||
               p->prim_type == PrimKind::Real ||
               p->prim_type == PrimKind::Char;
    }
    return false;
}

bool OperatorTypeSystem::is_array(type_ptr T) {
    return std::dynamic_pointer_cast<ArrayType>(T) != nullptr;
}

bool OperatorTypeSystem::is_slice(type_ptr T){
    return std::dynamic_pointer_cast<SliceType>(T) != nullptr;   
}

bool OperatorTypeSystem::is_tuple(type_ptr T) {
    return std::dynamic_pointer_cast<TupleType>(T) != nullptr;
}

bool OperatorTypeSystem::is_func(type_ptr T){
    return std::dynamic_pointer_cast<FunctionType>(T) != nullptr;
}

//
// PRIMITIVE RANKING
//
int OperatorTypeSystem::prim_rank(PrimKind k) {
    switch (k) {
        case PrimKind::Bool: return 0;
        case PrimKind::Char: return 1;
        case PrimKind::Int:  return 2;
        case PrimKind::Real: return 3;
        default:
            return -1;
    }
}

//
// PRIMITIVE PROMOTION
//
type_ptr OperatorTypeSystem::promote_primitive(
    std::shared_ptr<PrimType> A,
    std::shared_ptr<PrimType> B)
{
    int ra = prim_rank(A->prim_type);
    int rb = prim_rank(B->prim_type);

    PrimKind out = (ra >= rb) ? A->prim_type : B->prim_type;
    return std::make_shared<PrimType>(out);
}

//
// GENERAL PROMOTION
//
type_ptr OperatorTypeSystem::promote(type_ptr L, type_ptr R,
                                     const std::string& op)
{
    if (L->equals(R))
        return L;


    // both primitive
    if (auto pa = tc.to_prim(L))
        if (auto pb = tc.to_prim(R))
            return promote_primitive(pa, pb);

    // array + primitive => array(promoted elem)
    if (auto arr = tc.to_array(L))
        if (is_numeric(R)) {
            auto elem = promote(arr->array_type, R, op);
            return std::make_shared<ArrayType>(elem, arr->size);
        }

    if (auto arr = tc.to_array(R))
        if (is_numeric(L)) {
            auto elem = promote(arr->array_type, L, op);
            return std::make_shared<ArrayType>(elem, arr->size);
        }

    // array + array
    if (auto a1 = tc.to_array(L))
        if (auto a2 = tc.to_array(R)) {
            // for this check the sizes, and just add to the other array 
            auto elem = promote(a1->array_type, a2->array_type, op);
            return std::make_shared<ArrayType>(elem, std::max(a1->size , a2->size));
        }

    // tuple + tuple
    if (auto t1 = tc.to_tuple(L))
        if (auto t2 = tc.to_tuple(R)) {
            if (t1->tuple_types.size() != t2->tuple_types.size())
                throw std::runtime_error("Tuple size mismatch in op " + op);
            std::vector<type_ptr> elems;
            for (size_t i = 0; i < t1->tuple_types.size(); ++i)
                elems.push_back(promote(t1->tuple_types[i], t2->tuple_types[i], op));

            return std::make_shared<TupleType>(elems);
        }
    
    //   func + func 
    if (auto t1 = tc.to_func(L))
        if (auto t2 = tc.to_func(R)) {
           auto fn = promote(t1->return_type, t2->return_type, op);
           return fn;
        }

    // func + prim
      if (auto t1 = tc.to_func(L))
        if (auto t2 = tc.to_prim(R)) {
           auto fn = promote(t1->return_type, t2, op);
           return fn;
        }
      if (auto t1 = tc.to_func(R))
        if (auto t2 = tc.to_prim(L)) {
           auto fn = promote(t1->return_type, t2, op);
           return fn;
        }

    // func + array?
      if (auto t1 = tc.to_func(L))
        if (auto t2 = tc.to_array(R)) {
           auto fn = promote(t1->return_type, t2, op);
           return fn;
        }
      if (auto t1 = tc.to_func(R))
        if (auto t2 = tc.to_array(L)) {
           auto fn = promote(t1->return_type, t2, op);
           return fn;
        }
    



    throw std::runtime_error(
        "Cannot apply operator '" + op + "' to " +
        L->to_string() + " and " + R->to_string());
}

//
// UNARY OPS
//
type_ptr OperatorTypeSystem::check_unary(UniOp op, type_ptr operand)
{
    switch (op) {

    case UniOp::POS:
    case UniOp::NEG:
        if (!is_numeric(operand))
            throw std::runtime_error("Unary +/- requires numeric type");
        return operand;

    default:
        throw std::runtime_error("Unknown unary op");
    }
}

//
// RANGE (..) OPERATOR
//
type_ptr OperatorTypeSystem::check_range(type_ptr L, type_ptr R)
{
    if (!is_numeric(L) || !is_numeric(R))
        throw std::runtime_error("Range operator requires numeric types");
    if (!is_integer(L) || !is_integer(R))
        throw std::runtime_error("Range operator requires numeric INT types");

    return std::make_shared<RangeType>(L,R);
}

//
// PIPE (|>) OPERATOR
//
type_ptr OperatorTypeSystem::check_pipe(type_ptr value, type_ptr fnType)
{

    // there is a lot we need to do
    // for this project we are just going to focus on one parameter 2 |> f(x) = f(2); we are going to check how many parameters it accepts and the function that 
    // gets piped MUST only have 1 paraemeter, this is already too long as is, if we upgrade it in the future than sure, but for now keep it simple

    auto fn = std::dynamic_pointer_cast<FunctionType>(fnType);
   
    return fn->return_type;
}

//
// INDEXING
//
type_ptr OperatorTypeSystem::check_index(type_ptr base, type_ptr index)
{
    if (!is_array(base))
        throw std::runtime_error("Cannot index non-array type");

    if (!is_numeric(index) || !is_integer(index))
        throw std::runtime_error("Array index must be numeric (Int)");

    

    if (auto arr = std::dynamic_pointer_cast<ArrayType>(base))
        return arr->array_type;

    if (auto slice = std::dynamic_pointer_cast<SliceType>(base))
        return slice->slice_type;

    throw std::runtime_error("Unknown array-like type");
}

//
// BINARY OPS
//
type_ptr OperatorTypeSystem::check_binary(BinaryOp op,
                                          type_ptr L, type_ptr R)
{

    std::cout << "L = " << L->to_fundamental_string() << "  R = " << R->to_fundamental_string() << std::endl;

    if(is_arth(op)){
        // arithmetic
        return promote(L, R, binop_name(op));
    }

    if(is_comp(op)){
        // comparison
        auto re = promote(L, R, binop_name(op));
        if(is_array(L) || is_array(R)){
            tc.to_array(re)->array_type = std::make_shared<PrimType>(PrimKind::Bool);
            return re;
        }
        return re;
    }

    if(is_logic(op)){
        // logic
         if (!is_bool(L) || !is_bool(R))
            throw std::runtime_error("and/or require Bool operands");
        return promote(L, R, binop_name(op));
    }

    switch (op) {
    // range
    case BinaryOp::RANGE:
        return check_range(L, R);
    // pipe
    case BinaryOp::PIPE:
        return check_pipe(L, R);
    default:
        throw std::runtime_error("Unknown binary operator");
    }
}

