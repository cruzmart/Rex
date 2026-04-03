#include "passes/rex_assignment_pass.h"

namespace rex {

// ============================================
// ENTRY
// ============================================

void AssignmentCheckPass::visit(const std::shared_ptr<FileAst>& file) {
    for (auto& item : file->items) {

        if (item->ast_kind == AstNodeKind::Stmt) {
            visitStmt(std::static_pointer_cast<Stmt>(item));
        }

        else if (item->ast_kind == AstNodeKind::FunctionDecl) {
            auto fn = std::static_pointer_cast<FunctionDecl>(item);
            visitBlock(fn->body);
        }
    }
}

// ============================================
// BLOCK
// ============================================

void AssignmentCheckPass::visitBlock(const std::shared_ptr<BlockExpr>& block) {
    if (!block) return;

    for (auto& stmt : block->statements) {
        visitStmt(stmt);
    }
}

// ============================================
// STATEMENTS
// ============================================

void AssignmentCheckPass::visitStmt(const std::shared_ptr<Stmt>& stmt) {
    switch (stmt->stmt_kind) {

        case StmtKind::Assign:
            visitAssign(std::static_pointer_cast<AssignStmt>(stmt));
            break;

        case StmtKind::If:
            visitIf(std::static_pointer_cast<IfStmt>(stmt));
            break;

        case StmtKind::While:
            visitWhile(std::static_pointer_cast<WhileStmt>(stmt));
            break;

        case StmtKind::For:
            visitFor(std::static_pointer_cast<ForStmt>(stmt));
            break;

        default:
            break;
    }
}

// ============================================
// ASSIGNMENT
// ============================================

void AssignmentCheckPass::visitAssign(const std::shared_ptr<AssignStmt>& stmt) {

    auto target = stmt->target;
    auto value  = stmt->value;

    // ❌ Check assignable (LHS must be valid target)
    if (!isAssignable(target)) {
        err.error(stmt, "Invalid assignment target");
        return;
    }

    // Types should already be resolved from ExprPass
    auto target_type = target->type;
    auto value_type  = value->type;

    if (!target_type || !value_type) {
        err.error(stmt, "Internal error: unresolved type in assignment");
        return;
    }

    // ❌ Type mismatch
    if (!value_type->equals(target_type)) {
        err.error(stmt,
            "Type mismatch in assignment: cannot assign '" +
            value_type->to_string() +
            "' to '" +
            target_type->to_string() + "'"
        );
    }
}

// ============================================
// CONTROL FLOW
// ============================================

void AssignmentCheckPass::visitIf(const std::shared_ptr<IfStmt>& stmt) {
    visitBlock(stmt->then_block);

    for (auto& [cond, block] : stmt->elifx_blocks) {
        visitBlock(block);
    }

    if (stmt->else_block) {
        visitBlock(stmt->else_block);
    }
}

void AssignmentCheckPass::visitWhile(const std::shared_ptr<WhileStmt>& stmt) {
    visitBlock(stmt->body);
}

void AssignmentCheckPass::visitFor(const std::shared_ptr<ForStmt>& stmt) {
    visitBlock(stmt->body);
}

// ============================================
// HELPERS
// ============================================

bool AssignmentCheckPass::isAssignable(const std::shared_ptr<Expr>& expr) {

    switch (expr->exp_kind) {

        case ExprKind::Id:
            return true;

        case ExprKind::Index:
            return true;

        default:
            return false;
    }
}

}