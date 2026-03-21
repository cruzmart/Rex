#pragma once
#include <memory>
#include <string>



namespace rex {

struct Errors{
        public:
        // Inside ExprPass
            template <typename T>
            [[noreturn]] void error(const std::shared_ptr<T>& node, const std::string& msg) const {
                // T must have loc field (AstNode-derived)
                throw std::runtime_error(
                    "Error at line " + std::to_string(node->loc.line) +
                    ", column " + std::to_string(node->loc.column) +
                    ": " + msg
                );
            }

    };
}