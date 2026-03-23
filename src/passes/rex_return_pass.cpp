#include "passes/rex_return_pass.h"

namespace rex {

// ============================================
// ENTRY
// ============================================

void ReturnCheckPass::visit(const std::shared_ptr<FileAst>& file) {
    for (auto& item : file->items) {
        if (item->ast_kind == AstNodeKind::FunctionDecl) {
            visitFunction(std::static_pointer_cast<FunctionDecl>(item));
        }
    }
}

// ============================================
// FUNCTION
// ============================================

void ReturnCheckPass::visitFunction(const std::shared_ptr<FunctionDecl>& fn) {
    current_return_type = fn->func_type->ret;

    bool always_returns = visitBlock(fn->body);

    bool is_void = isVoid(current_return_type);

    if (!is_void && !always_returns) {
        err.error(fn, "Function '" + fn->func_name + "' does not return on all paths");
    }
}

// ============================================
// HELPERS
// ============================================

bool ReturnCheckPass::isVoid(const std::shared_ptr<Type>& t) {
    return t->kind == TypeKind::Primitive &&
           std::static_pointer_cast<PrimType>(t)->prim == PrimType::Prims::Void;
}

// ============================================
// BLOCK
// ============================================

bool ReturnCheckPass::visitBlock(const std::shared_ptr<BlockExpr>& block) {
    bool has_returned = false;

    for (size_t i = 0; i < block->statements.size(); i++) {
        auto& stmt = block->statements[i];

        if (has_returned) {
            err.error(stmt, "Unreachable statement after return");
            continue;
        }

        bool stmt_returns = visitStmt(stmt);

        // 🔥 implicit return (ONLY last statement)
        if (!stmt_returns && i == block->statements.size() - 1) {
            if (stmt->stmt_kind == StmtKind::Return_Expr) {
                auto es = std::static_pointer_cast<ExprStmt>(stmt);

                if (isVoid(current_return_type)) {
                    err.error(es, "Void function cannot return a value");
                } else {
                    if (!es->value->type->equals(current_return_type)) {
                        err.error(es, "Return type mismatch (implicit return)");
                    }
                }

                stmt_returns = true;
            }
        }

        if (stmt_returns) {
            has_returned = true;
        }
    }

    return has_returned;
}

// ============================================
// STATEMENTS
// ============================================

bool ReturnCheckPass::visitStmt(const std::shared_ptr<Stmt>& stmt) {
    switch (stmt->stmt_kind) {
        case StmtKind::Return_Normal:
            return visitReturn(std::static_pointer_cast<ReturnStmt>(stmt));

        case StmtKind::If:
            return visitIf(std::static_pointer_cast<IfStmt>(stmt));

        case StmtKind::While:
            return visitWhile(std::static_pointer_cast<WhileStmt>(stmt));

        case StmtKind::For:
            return visitFor(std::static_pointer_cast<ForStmt>(stmt));

        case StmtKind::Return_Expr:
            return false; // handled in block as implicit return

        case StmtKind::Break:
            return false;

        default:
            return false;
    }
}

// ============================================
// RETURN
// ============================================

bool ReturnCheckPass::visitReturn(const std::shared_ptr<ReturnStmt>& stmt) {
    bool is_void = isVoid(current_return_type);

    if (is_void) {
        if (stmt->value) {
            err.error(stmt, "Void function cannot return a value");
        }
    } else {
        if (!stmt->value) {
            err.error(stmt, "Non-void function must return a value");
        } else {
            if (!stmt->value->type->equals(current_return_type)) {
                err.error(stmt, "Return type mismatch");
            }
        }
    }

    return true;
}

// ============================================
// IF
// ============================================

bool ReturnCheckPass::visitIf(const std::shared_ptr<IfStmt>& stmt) {
    bool then_returns = visitBlock(stmt->then_block);

    bool all_elif_return = true;
    for (auto& [cond, block] : stmt->elifx_blocks) {
        if (!visitBlock(block)) {
            all_elif_return = false;
        }
    }

    if (!stmt->else_block) return false;

    bool else_returns = visitBlock(stmt->else_block);

    return then_returns && all_elif_return && else_returns;
}

// ============================================
// LOOPS
// ============================================

bool ReturnCheckPass::visitWhile(const std::shared_ptr<WhileStmt>& stmt) {
    visitBlock(stmt->body);
    return false; // loop may not execute
}

bool ReturnCheckPass::visitFor(const std::shared_ptr<ForStmt>& stmt) {
    visitBlock(stmt->body);
    return false;
}

} // namespace rex