#include "passes/rex_break_pass.h"

namespace rex {

void BreakCheckPass::visit(const std::shared_ptr<FileAst>& file) {
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

void BreakCheckPass::visitBlock(const std::shared_ptr<BlockExpr>& block) {
    if (!block) return;

    for (auto stmt : block->statements) {
        visitStmt(stmt);
    }
}

void BreakCheckPass::visitStmt(const std::shared_ptr<Stmt>& stmt) {
    switch (stmt->stmt_kind) {

        case StmtKind::While:
            visitWhile(std::static_pointer_cast<WhileStmt>(stmt));
            break;

        case StmtKind::For:
            visitFor(std::static_pointer_cast<ForStmt>(stmt));
            break;

        case StmtKind::Loop:
            visitLoop(std::static_pointer_cast<LoopStmt>(stmt));
            break;

        case StmtKind::If:
            visitIf(std::static_pointer_cast<IfStmt>(stmt));
            break;

        case StmtKind::Break:
            visitBreak(stmt);
            break;

        default:
            // other statements don’t matter
            break;
    }
}

void BreakCheckPass::visitWhile(const std::shared_ptr<WhileStmt>& stmt) {
    loop_depth++;

    visitBlock(stmt->body);

    loop_depth--;
}

void BreakCheckPass::visitFor(const std::shared_ptr<ForStmt>& stmt) {
    loop_depth++;

    visitBlock(stmt->body);

    loop_depth--;
}

void BreakCheckPass::visitLoop(const std::shared_ptr<LoopStmt>& stmt) {
    loop_depth++;

    visitBlock(stmt->body);

    loop_depth--;
}

void BreakCheckPass::visitIf(const std::shared_ptr<IfStmt>& stmt) {
    visitBlock(stmt->then_block);

    for (auto& [cond, block] : stmt->elifx_blocks) {
        visitBlock(block);
    }

    if (stmt->else_block) {
        visitBlock(stmt->else_block);
    }
}

void BreakCheckPass::visitBreak(const std::shared_ptr<Stmt>& stmt) {
    if (loop_depth == 0) {
        err.error(stmt, "'break' used outside of a loop");
    }
}

}