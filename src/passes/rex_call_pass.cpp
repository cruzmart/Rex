#include "passes/rex_call_pass.h"

namespace rex {

namespace {

template <typename T, typename U>
std::shared_ptr<T> as(const std::shared_ptr<U>& ptr) {
    return std::dynamic_pointer_cast<T>(ptr);
}

bool isVoidType(const std::shared_ptr<Type>& type) {

    if (!type) {
        return false;
    }

    if (type->kind != TypeKind::Primitive) {
        return false;
    }

    auto prim = as<PrimType>(type);

    if (!prim) {
        return false;
    }

    return prim->prim == PrimType::Prims::Void;
}

} // namespace

// =====================================================
// ENTRY
// =====================================================

void FunctionCallPass::visit(std::shared_ptr<FileAst> file) {

    if (!file) {
        return;
    }

    for (const auto& item : file->items) {

        if (!item) {
            continue;
        }

        switch (item->ast_kind) {

            case AstNodeKind::Stmt:
                visitStmt(as<Stmt>(item));
                break;

            case AstNodeKind::FunctionDecl:
                visitFunction(as<FunctionDecl>(item));
                break;

            default:
                break;
        }
    }
}

// =====================================================
// HELPERS
// =====================================================

bool FunctionCallPass::isVoid(std::shared_ptr<CallExpr> call) {

    if (!call) {
        return false;
    }

    return isVoidType(call->type);
}

// =====================================================
// BLOCKS
// =====================================================

void FunctionCallPass::visitBlock(
    std::shared_ptr<BlockExpr> block
) {

    if (!block) {
        return;
    }

    for (auto stmt : block->statements) {

        if (!stmt) {
            continue;
        }

        visitStmt(stmt);
    }
}

// =====================================================
// FUNCTIONS
// =====================================================

void FunctionCallPass::visitFunction(
    std::shared_ptr<FunctionDecl> func
) {

    if (!func) {
        return;
    }

    visitBlock(func->body);
}

// =====================================================
// STATEMENTS
// =====================================================

void FunctionCallPass::visitStmt(
    std::shared_ptr<Stmt> stmt
) {

    if (!stmt) {
        return;
    }

    switch (stmt->stmt_kind) {

        case StmtKind::If:
            visitIf(as<IfStmt>(stmt));
            return;

        case StmtKind::While:
            visitWhile(as<WhileStmt>(stmt));
            return;

        case StmtKind::For:
            visitFor(as<ForStmt>(stmt));
            return;

        case StmtKind::Loop:
            visitLoop(as<LoopStmt>(stmt));
            return;

        case StmtKind::Return_Expr:
            break;

        default:
            return;
    }

    // =====================================================
    // RETURN EXPR HANDLING
    // =====================================================

    auto exprStmt = as<ExprStmt>(stmt);

    if (!exprStmt || !exprStmt->value) {
        return;
    }

    if (exprStmt->value->exp_kind != ExprKind::Call) {
        return;
    }

    auto call = as<CallExpr>(exprStmt->value);

    if (!call) {
        return;
    }

    // Convert:
    // return foo();
    //
    // into:
    // foo();
    //
    // if foo returns void.

    if (isVoid(call)) {
        stmt->stmt_kind = StmtKind::Void_Call;
    }
}

// =====================================================
// IF
// =====================================================

void FunctionCallPass::visitIf(
    std::shared_ptr<IfStmt> if_stmt
) {

    if (!if_stmt) {
        return;
    }

    if (if_stmt->then_block) {
        visitBlock(if_stmt->then_block);
    }

    for (auto& block : if_stmt->elifx_blocks) {

        if (block.second) {
            visitBlock(block.second);
        }
    }

    if (if_stmt->else_block) {
        visitBlock(if_stmt->else_block);
    }
}

// =====================================================
// WHILE
// =====================================================

void FunctionCallPass::visitWhile(
    std::shared_ptr<WhileStmt> whle_stmt
) {

    if (!whle_stmt) {
        return;
    }

    visitBlock(whle_stmt->body);
}

// =====================================================
// FOR
// =====================================================

void FunctionCallPass::visitFor(
    std::shared_ptr<ForStmt> for_stmt
) {

    if (!for_stmt) {
        return;
    }

    visitBlock(for_stmt->body);
}

// =====================================================
// LOOP
// =====================================================

void FunctionCallPass::visitLoop(
    std::shared_ptr<LoopStmt> lop_stmt
) {

    if (!lop_stmt) {
        return;
    }

    visitBlock(lop_stmt->body);
}

} // namespace rex