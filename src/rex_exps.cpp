#include "rex_exps.h"
#include "rex_stmts.h"
#include "rex_ast_nodes.h"
#include "rex_types.h"
#include <memory>

namespace rex {

void BlockExpr::dump(std::ostream& os, int i) const {
    indent(os, i); os << "block\n";
    for (auto& s : statements) s->dump(os, i + 1);
    if (result) { indent(os, i + 1); os << "result\n"; result->dump(os, i + 2); }
}

// IdExpr
void IdExpr::dump(std::ostream& os, int i) const { indent(os, i); os << "id " << name << "\n"; }

// LiteralExpr
void LiteralExpr::dump(std::ostream& os, int i) const {
    indent(os, i);
    if(auto prim = std::dynamic_pointer_cast<PrimType>(type)) os << "literal " << value << " -> " << prim->to_string() << " : " << type->to_fundamental_string() << "\n";
    else os << "literal " << value << " -> <?> : " << (type ? type->to_fundamental_string() : "<null>") << "\n";
}

// UnaryExpr

void UnaryExpr::dump(std::ostream& os, int i) const {
    indent(os, i); os << "unary " << uniop_name(operation) << "\n";
    if(rhs) rhs->dump(os, i + 1);
}

// BinaryExpr
void BinaryExpr::dump(std::ostream& os, int i) const {
    indent(os, i); os << "binary " << binop_name(operation) << " : " << (type ? type->to_fundamental_string() : "<null>") << "\n";
    if(lhs) lhs->dump(os, i + 1);
    if(rhs) rhs->dump(os, i + 1);
}

// RangeExpr / PipeExpr
void RangeExpr::dump(std::ostream& os, int i) const { indent(os, i); os << "range\n"; if(lhs) lhs->dump(os, i + 1); if(rhs) rhs->dump(os, i + 1); }
void PipeExpr::dump(std::ostream& os, int i) const { indent(os, i); os << "pipe\n"; if(lhs) lhs->dump(os, i + 1); if(rhs) rhs->dump(os, i + 1); }

// CallExpr
void CallExpr::dump(std::ostream& os, int i) const {
    indent(os, i); os << "call " << callee << "\n";
    for(auto& a : args) a->dump(os, i + 1);
}

// IndexExpr
void IndexExpr::dump(std::ostream& os, int i) const {
    indent(os, i); os << "index\n";
    indent(os, i + 1); os << "base\n"; if(base) base->dump(os, i + 2);
    indent(os, i + 1); os << "index\n"; if(index) index->dump(os, i + 2);
}

// TupleExpr / ArrayExpr
void TupleExpr::dump(std::ostream& os, int i) const { indent(os, i); os << "tuple\n"; for(auto& e : elements) e->dump(os, i + 1); }
void ArrayExpr::dump(std::ostream& os, int i) const { indent(os, i); os << "array -> " + type->to_string() + "\n"; for(auto& e : elements) e->dump(os, i + 1); }

} // namespace rex