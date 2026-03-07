#include "rex_operator_type_system.h"
#include "rex_ast_nodes.h"
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

//
// PRIMITIVE RANKING
//
int OperatorTypeSystem::prim_rank(PrimKind k) {
    switch (k) {
        case PrimKind::Bool: return 0;
        case PrimKind::Char: return 1;
        case PrimKind::Int:  return 2;
        case PrimKind::Real: return 3;
    }
    return -1;
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

    //
    // both primitive
    //
    if (auto pa = std::dynamic_pointer_cast<PrimType>(L))
        if (auto pb = std::dynamic_pointer_cast<PrimType>(R))
            return promote_primitive(pa, pb);

    //
    // array + primitive => array(promoted elem)
    //
    if (auto arr = std::dynamic_pointer_cast<ArrayType>(L))
        if (is_numeric(R)) {
            auto elem = promote(arr->array_type, R, op);
            return std::make_shared<ArrayType>(elem, arr->size);
        }

    if (auto arr = std::dynamic_pointer_cast<ArrayType>(R))
        if (is_numeric(L)) {
            auto elem = promote(arr->array_type, L, op);
            return std::make_shared<ArrayType>(elem, arr->size);
        }

    //
    // array + array
    //
    if (auto a1 = std::dynamic_pointer_cast<ArrayType>(L))
        if (auto a2 = std::dynamic_pointer_cast<ArrayType>(R)) {
            // for this check the sizes, and just add to the other array 
            auto elem = promote(a1->array_type, a2->array_type, op);
            return std::make_shared<ArrayType>(elem, a1->size + a2->size);
        }

    //
    // tuple + tuple
    //
    if (auto t1 = std::dynamic_pointer_cast<TupleType>(L))
        if (auto t2 = std::dynamic_pointer_cast<TupleType>(R)) {
            if (t1->tuple_types.size() != t2->tuple_types.size())
                throw std::runtime_error("Tuple size mismatch in op " + op);

            std::vector<type_ptr> elems;
            for (size_t i = 0; i < t1->tuple_types.size(); ++i)
                elems.push_back(promote(t1->tuple_types[i], t2->tuple_types[i], op));

            return std::make_shared<TupleType>(elems);
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
    auto fn = std::dynamic_pointer_cast<FunctionType>(fnType);
    if (!fn)
        throw std::runtime_error("Right side of '|>' must be a function");

    if (fn->params_type.size() != 1)
        throw std::runtime_error("Pipe expects a unary function");

    // check if argument is compatible
    promote(value, fn->params_type[0], "|>");

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
    switch (op) {

    // arithmetic
    case BinaryOp::ADD:
    case BinaryOp::SUB:
    case BinaryOp::MUL:
    case BinaryOp::DIV:
    case BinaryOp::MOD:
        return promote(L, R, binop_name(op));

    // comparisons
    case BinaryOp::LT:
    case BinaryOp::GT:
    case BinaryOp::LE:
    case BinaryOp::GE:
        if(is_array(L) || is_array(R))
            return promote(L, R, binop_name(op)); // just ensure comparable
        return std::make_shared<PrimType>(PrimKind::Bool);

    // equality
    case BinaryOp::EQ:
    case BinaryOp::NEQ:
        return promote(L, R, binop_name(op));
        // return std::make_shared<PrimType>(PrimKind::Bool);

    // logical
    case BinaryOp::AND:
    case BinaryOp::OR:
        if (!is_bool(L) || !is_bool(R))
            throw std::runtime_error("and/or require Bool operands");
        return std::make_shared<PrimType>(PrimKind::Bool);

    // range
    case BinaryOp::RANGE:
        return check_range(L, R);

    // pipe
    case BinaryOp::PIPE:
        return check_pipe(L, R);
    }

    throw std::runtime_error("Unknown binary operator");
}

