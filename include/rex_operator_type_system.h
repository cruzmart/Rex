#pragma once
#include "rex_ast_nodes.h"
#include "rex_types.h"
#include <memory>
#include <string>

namespace rex {

struct OperatorTypeSystem {

    // Debug toggle
    bool debug = false;

    // ===== Main API =====
    type_ptr check_unary(UniOp op, type_ptr operand);
    type_ptr check_binary(BinaryOp op, type_ptr L, type_ptr R);

    type_ptr check_index(type_ptr base, type_ptr index);
    type_ptr check_pipe(type_ptr value, type_ptr fn);
    type_ptr check_range(type_ptr L, type_ptr R);

    // Promotion / helper
    type_ptr promote(type_ptr L, type_ptr R, const std::string& op);

private:
    // helpers
    bool is_primitive(type_ptr T);
    bool is_numeric(type_ptr T);
    bool is_bool(type_ptr T);
    bool is_array(type_ptr T);
    bool is_tuple(type_ptr T);

    int prim_rank(PrimType::Prims k);

    type_ptr promote_primitive(std::shared_ptr<PrimType> A,
                               std::shared_ptr<PrimType> B);

};

} // namespace rex