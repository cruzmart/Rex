#pragma once
#include "rex_ast_nodes.h"
#include "rex_ops.h"
#include "rex_types.h"
#include <memory>
#include <string>
#include <algorithm>

#include "rex_errors.h"
#include "rex_exps.h"

namespace rex {

struct BinaryOpSystem {
    // Debug toggle
    bool debug = false;
    Errors errors;


    // ===== Main API =====
    type_ptr check_unary(UniOp op, type_ptr operand);
    type_ptr check_binary(std::shared_ptr<BinaryExpr> exp, BinaryOp op, type_ptr L, type_ptr R);

    type_ptr check_index(type_ptr base, type_ptr index);
    type_ptr check_pipe(type_ptr value, type_ptr fn);
    type_ptr check_range(type_ptr L, type_ptr R);

    type_ptr promote(type_ptr L, type_ptr R, const std::string& op);

    // ===== Helpers =====
    inline bool is_prim(type_ptr t, rex::PrimType::Prims k);
    bool is_primitive(type_ptr T);
    bool is_numeric(type_ptr T);
    bool is_bool(type_ptr T);
    bool is_array(type_ptr T);
    bool is_tuple(type_ptr T);
    bool is_slice(type_ptr T);
    bool is_func(type_ptr T);

    bool is_integer(type_ptr T);
    bool is_real(type_ptr T);
    bool is_string(type_ptr T);
    bool is_char(type_ptr T);

    bool is_comp(BinaryOp op);
    bool is_arth(BinaryOp op);
    bool is_logic(BinaryOp op);

    int prim_rank(PrimType::Prims k);
    type_ptr promote_primitive(std::shared_ptr<PrimType> A, std::shared_ptr<PrimType> B);

};

} // namespace rex