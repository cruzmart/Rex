#pragma once

#include <memory>

#include "rex_ast.h"
#include "rex_ast_nodes.h"
#include "rex_errors.h"
#include "rex_stmts.h"
#include "rex_funcs.h"
#include "rex_exps.h"

namespace rex {

    struct TopLevelOrderPass {
    private:
        Errors err;
        int countFunctions(const std::shared_ptr<FileAst>& file); 
        int countAliasing(const std::shared_ptr<FileAst>& file);

    public:
        void visit(const std::shared_ptr<FileAst>& file);
    };

}