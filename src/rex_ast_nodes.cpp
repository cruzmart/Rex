#include "rex_ast_nodes.h"
namespace rex {

void FileAst::dump(std::ostream& os, int i) const {
    indent(os, i);
    os << "file\n";
    for (auto& it : items)
        it->dump(os, i + 1);
}

const char* binop_name(BinaryOp o) {
    switch (o) {
        case BinaryOp::ADD: return "+";
        case BinaryOp::SUB: return "-";
        case BinaryOp::MUL: return "*";
        case BinaryOp::DIV: return "/";
        case BinaryOp::MOD: return "%";
        case BinaryOp::EQ: return "==";
        case BinaryOp::NEQ: return "!=";
        case BinaryOp::LT: return "<";
        case BinaryOp::GT: return ">";
        case BinaryOp::LE: return "<=";
        case BinaryOp::GE: return ">=";
        case BinaryOp::AND: return "and";
        case BinaryOp::OR: return "or";
        case BinaryOp::PIPE: return "|>";
        case BinaryOp::RANGE: return "..";
    }
    return "?";
}

const char* uniop_name(UniOp o) {
    switch (o) {
        case UniOp::POS: return "+";
        case UniOp::NEG: return "-";
    }
    return "?";
}

} // namespace rex