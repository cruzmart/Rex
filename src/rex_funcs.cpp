#include "rex_funcs.h"
#include "rex_exps.h"
#include "rex_types.h"

namespace rex {


void Parameter::dump(std::ostream& os, int i) const {
    indent(os, i); os << "param " << para_name;
    if (para_type) os << " : " << para_type->to_fundamental_string();
    else os << " : <unknown type>";
    if (resolved) os << " [resolved]";
    os << "\n";
}

void FunctionDecl::dump(std::ostream& os, int i) const {
    indent(os, i); os << "function " << func_name;
    if (func_type) os << " -> " << func_type->to_string();
    if (resolved) os << " [resolved]";
    os << "\n";

    if (!func_type->params.empty()) {
        indent(os, i + 1); os << "parameters:\n";
        for (auto& p : func_type->params) if (p) p->dump(os, i + 2);
    }

    if (body) {
        indent(os, i + 1); os << "body:\n";
        body->dump(os, i + 2);
    }
}

} // namespace rex