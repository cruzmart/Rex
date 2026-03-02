 #include "rex_stmts.h"
#include "rex_exps.h"
#include "rex_ast.h"

namespace rex {

// ---------------------- PATTERNS --------------------------
PatternId::PatternId(std::string id) : id(std::move(id)) {}

// ---------------------- STATEMENTS ------------------------
void LetStmt::dump(std::ostream& os, int i) const {
    indent(os, i); os << "let:\n";

    indent(os, i + 1); os << "pattern: ";
    if (auto p = std::dynamic_pointer_cast<PatternId>(id_pattern)) {
        os << p->id << "\n";
    } else if (auto p = std::dynamic_pointer_cast<PatternIds>(id_pattern)) {
        for (size_t idx = 0; idx < p->ids.size(); ++idx) {
            os << p->ids[idx];
            if (idx < p->ids.size() - 1) os << ", ";
        }
        os << "\n";
    } else {
        os << "<unknown pattern>\n";
    }

    if (type) { indent(os, i + 1); os << "type: " << type->to_string() << "\n"; }
    if (exp) { indent(os, i + 1); os << "initializer:\n"; exp->dump(os, i + 2); }
}

void TypeDecl::dump(std::ostream& os, int i) const {
    indent(os, i); os << "new type: " << name;
    if (type) os << " -> " << type->to_string();
    else os << " -> <error>";
    if (resolved) os << " [resolved]";
    os << "\n";
}

void AssignStmt::dump(std::ostream& os, int i) const {
    indent(os, i); os << "assign\n";
    target->dump(os, i + 1);
    value->dump(os, i + 1);
}

void ReturnStmt::dump(std::ostream& os, int i) const {
    indent(os, i); os << "return\n";
    if (value) value->dump(os, i + 1);
}

void ExprStmt::dump(std::ostream& os, int i) const {
    indent(os, i); os << "expr_stmt\n";
    value->dump(os, i + 1);
}

void WhileStmt::dump(std::ostream& os, int i) const {
    indent(os, i); os << "while\n";
    cond->dump(os, i + 1);
    body->dump(os, i + 1);
}

void ForStmt::dump(std::ostream& os, int i) const {
    indent(os, i); os << "for " << iter_var << " in\n";
    iterable->dump(os, i + 1);
    body->dump(os, i + 1);
}

void LoopStmt::dump(std::ostream& os, int i) const {
    indent(os, i); os << "loop\n";
    body->dump(os, i + 1);
}

void IfStmt::dump(std::ostream& os, int i) const {
    indent(os, i); os << "if\n";
    indent(os, i + 1); os << "condition\n"; condition->dump(os, i + 2);
    indent(os, i + 1); os << "then\n"; then_block->dump(os, i + 2);
    for (const auto& [elifCond, elifBlock] : elifx_blocks) {
        indent(os, i + 1); os << "elifx\n";
        indent(os, i + 2); os << "condition\n"; elifCond->dump(os, i + 3);
        indent(os, i + 2); os << "then\n"; elifBlock->dump(os, i + 3);
    }
    if (else_block) { indent(os, i + 1); os << "else\n"; else_block->dump(os, i + 2); }
}

} // namespace rex