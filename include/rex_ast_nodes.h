#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include "rex_ast.h"
#include "rex_ops.h"

namespace rex {

struct FileAst : AstNode {
    std::vector<std::shared_ptr<AstNode>> items;
    void dump(std::ostream& os, int i) const override;
};

const char* binop_name(BinaryOp o);
const char* uniop_name(UniOp o);

} // namespace rex