#include "rex_funcs.h"
#include "rex_exps.h"
#include "rex_types.h"

namespace rex {

Parameter::Parameter() : para_name(""), para_type(std::make_shared<Type>()), resolved(nullptr) {}
Parameter::Parameter(std::string name, std::shared_ptr<Type> type) : para_name(name), para_type(type), resolved(nullptr) {}

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

    if (!func_type->params_type.empty()) {
        indent(os, i + 1); os << "parameters:\n";
        for (auto& p : func_type->params_type) if (p) p->dump(os, i + 2);
    }

    if (body) {
        indent(os, i + 1); os << "body:\n";
        body->dump(os, i + 2);
    }
}

} // namespace rex