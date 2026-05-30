#pragma once

#include "rex_ast.h"
#include "rex_ast_nodes.h"
#include "rex_errors.h"
#include "rex_stmts.h"
#include "rex_exps.h"
#include "rex_types.h"
#include "rex_errors.h"

#include <memory>


namespace rex {

    struct FunctionCallPass {
        private:
            Errors errs;

        public:
            void visitStmt(std::shared_ptr<Stmt> stmt);
            void visit(std::shared_ptr<FileAst> file);
            bool isVoid(std::shared_ptr<CallExpr> cal);
            void visitBlock(std::shared_ptr<BlockExpr> block);
            void visitFunction(std::shared_ptr<FunctionDecl> func);
            void visitIf(std::shared_ptr<IfStmt> ifc);
            void visitWhile(std::shared_ptr<WhileStmt> whle);
            void visitFor(std::shared_ptr<ForStmt> f4r);
            void visitLoop(std::shared_ptr<LoopStmt> lop);

    };

}