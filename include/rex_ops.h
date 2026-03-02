#pragma once
namespace rex {

enum class BinaryOp {
    ADD, SUB, MUL, DIV, MOD, EQ, NEQ, LT, GT, LE, GE, AND, OR, PIPE, RANGE
};

enum class UniOp {
    POS, NEG
};

} // namespace rex