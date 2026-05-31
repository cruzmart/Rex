#include "passes/rex_toplevel_order_pass.h"

namespace rex {

    enum class HoistPhase {
        Aliases,
        Functions,
        Statements
    };

    void TopLevelOrderPass::visit(const std::shared_ptr<FileAst>& file) {

        HoistPhase phase = HoistPhase::Aliases;

        for (const auto& item : file->items) {

            //
            // Type Alias
            //
            if (item->ast_kind == AstNodeKind::Stmt) {

                auto stmt = std::static_pointer_cast<Stmt>(item);

                if (stmt->stmt_kind == StmtKind::TypeDecl) {

                    //
                    // Type aliases are ONLY allowed
                    // before functions/statements
                    //
                    if (phase != HoistPhase::Aliases) {
                        err.error(
                            item,
                            "Type aliases must be declared before function declarations and statements"
                        );
                    }

                    continue;
                }
            }

            //
            // Function Declaration
            //
            if (item->ast_kind == AstNodeKind::FunctionDecl) {

                //
                // Once we see a function,
                // we are now in function phase
                //
                if (phase == HoistPhase::Aliases) {
                    phase = HoistPhase::Functions;
                }

                //
                // Functions cannot appear
                // after executable statements
                //
                if (phase == HoistPhase::Statements) {
                    err.error(
                        item,
                        "Function declarations must appear before executable statements"
                    );
                }

                continue;
            }

            //
            // Any other node counts as a statement
            //
            phase = HoistPhase::Statements;
        }
    }

    int TopLevelOrderPass::countFunctions(const std::shared_ptr<FileAst>& file) {
        int total = 0;

        for (const auto& item : file->items) {
            if (item->ast_kind == AstNodeKind::FunctionDecl) {
                total++;
            }
        }

        return total;
    }

    int TopLevelOrderPass::countAliasing(const std::shared_ptr<FileAst>& file) {
        int total = 0;

        for (const auto& item : file->items) {

            if (item->ast_kind == AstNodeKind::Stmt) {

                auto stmt = std::static_pointer_cast<Stmt>(item);

                if (stmt->stmt_kind == StmtKind::TypeDecl) {
                    total++;
                }
            }
        }

        return total;
    }

}