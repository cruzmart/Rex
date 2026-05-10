#include "backend/rex_backend_prints.h"
#include "backend/rex_backend_types.h"

#include <llvm/Support/Error.h>

#include <mlir/Dialect/Arith/IR/Arith.h>
#include <mlir/Dialect/LLVMIR/LLVMDialect.h>
#include <mlir/Dialect/LLVMIR/LLVMTypes.h>
#include <mlir/Dialect/SCF/IR/SCF.h>

namespace rex {

PrintHelper::PrintHelper(
    std::shared_ptr<mlir::OpBuilder> b,
    mlir::Location l,
    std::shared_ptr<TypesHelper> t)
    : builder(std::move(b)),
      loc(l),
      types(std::move(t)) {}

/// =============================================================
/// Helpers
/// =============================================================

mlir::LLVM::AddressOfOp
PrintHelper::getFmtAddress(mlir::LLVM::GlobalOp fmt) {
    return builder->create<mlir::LLVM::AddressOfOp>(loc, fmt);
}

mlir::Value PrintHelper::i32(int value) {
    return builder->create<mlir::arith::ConstantIntOp>(
        loc,
        value,
        32
    );
}

mlir::Value PrintHelper::i8(char value) {
    return builder->create<mlir::arith::ConstantIntOp>(
        loc,
        value,
        8
    );
}

void PrintHelper::emitPrintf(
    mlir::Value fmt,
    mlir::Value value
) {
    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmt, value}
    );
}

void PrintHelper::emitChar(char c) {
    emitPrintf(getFmtAddress(fmt_char), i8(c));
}

void PrintHelper::emitSeparator() {
    emitChar(',');
    emitChar(' ');
}

template<typename Fn>
void PrintHelper::forLoop(
    mlir::Value upperBound,
    Fn &&body
) {
    auto loop = builder->create<mlir::scf::ForOp>(
        loc,
        i32(0),
        upperBound,
        i32(1)
    );

    builder->setInsertionPointToStart(loop.getBody());

    body(loop.getInductionVar());

    builder->setInsertionPointAfter(loop);
}

template<typename Fn>
void PrintHelper::emitIfNotLast(
    mlir::Value index,
    mlir::Value size,
    Fn &&body
) {
    auto lastIndex =
        builder->create<mlir::arith::SubIOp>(
            loc,
            size,
            i32(1)
        );

    auto condition =
        builder->create<mlir::arith::CmpIOp>(
            loc,
            mlir::arith::CmpIPredicate::ne,
            index,
            lastIndex
        );

    auto ifOp = builder->create<mlir::scf::IfOp>(
        loc,
        condition,
        false
    );

    builder->setInsertionPointToStart(
        &ifOp.getThenRegion().front()
    );

    body();

    builder->setInsertionPointAfter(ifOp);
}

/// =============================================================
/// Primitive Printing
/// =============================================================

void PrintHelper::printInline(mlir::Value value) {

    auto type = value.getType();

    mlir::Value fmt;

    if (type.isInteger(1)) {

        value = builder->create<mlir::arith::ExtUIOp>(
            loc,
            builder->getI32Type(),
            value
        );

        fmt = getFmtAddress(fmt_int);
    }
    else if (type.isInteger(32)) {
        fmt = getFmtAddress(fmt_int);
    }
    else if (type.isInteger(8)) {
        fmt = getFmtAddress(fmt_char);
    }
    else if (type.isF32()) {

        value = builder->create<mlir::arith::ExtFOp>(
            loc,
            builder->getF64Type(),
            value
        );

        fmt = getFmtAddress(fmt_float);
    }
    else if (type.isa<mlir::LLVM::LLVMPointerType>()) {
        fmt = getFmtAddress(fmt_string);
    }
    else {
        llvm::report_fatal_error(
            "Unsupported type in printInline"
        );
    }

    emitPrintf(fmt, value);
}

/// =============================================================
/// Generic Value Dispatcher
/// =============================================================

void PrintHelper::printValue(
    mlir::Value value,
    std::shared_ptr<Type> type
) {
    switch (type->kind) {

        case TypeKind::Primitive:
            printInline(value);
            return;

        case TypeKind::Array:
            printArray(
                value,
                std::static_pointer_cast<ArrayType>(type)
            );
            return;

        default:
            llvm::report_fatal_error(
                "Unsupported type in printValue"
            );
    }
}

/// =============================================================
/// Arrays
/// =============================================================

void PrintHelper::printArray(
    mlir::Value arrayPtr,
    std::shared_ptr<ArrayType> arrType
) {
    if (arrType->isMatrix()) {
        printMatrix(arrayPtr, arrType);
    }
    else {
        printFlatArray(arrayPtr, arrType);
    }
}

void PrintHelper::printFlatArray(
    mlir::Value arrayPtr,
    std::shared_ptr<ArrayType> arrType
) {
    auto ctx = builder->getContext();

    auto elemTy =
        types->getMLIRType(arrType->elem);

    auto [rows, cols] =
        arrType->dimensions();

    auto totalSize = i32(rows * cols);

    emitChar('[');

    forLoop(totalSize, [&](mlir::Value i) {

        auto elemPtr =
            builder->create<mlir::LLVM::GEPOp>(
                loc,
                mlir::LLVM::LLVMPointerType::get(ctx),
                elemTy,
                arrayPtr,
                mlir::ValueRange{i}
            );

        auto elem =
            builder->create<mlir::LLVM::LoadOp>(
                loc,
                elemTy,
                elemPtr
            );

        printInline(elem);

        emitIfNotLast(i, totalSize, [&] {
            emitSeparator();
        });
    });

    emitChar(']');
}

void PrintHelper::printMatrix(
    mlir::Value arrayPtr,
    std::shared_ptr<ArrayType> arrType
) {
    auto ctx = builder->getContext();

    auto [rows, cols] =
        arrType->dimensions();

    auto rowCount = i32(rows);
    auto colCount = i32(cols);

    auto matrixTy =
        types->getMLIRType(arrType);

    auto elemTy =
        types->getMLIRType(
            std::static_pointer_cast<ArrayType>(
                arrType->elem
            )->elem
        );

    emitChar('[');

    forLoop(rowCount, [&](mlir::Value row) {

        emitChar('[');

        forLoop(colCount, [&](mlir::Value col) {

            auto rowOffset =
                builder->create<mlir::arith::MulIOp>(
                    loc,
                    row,
                    colCount
                );

            auto linearIndex =
                builder->create<mlir::arith::AddIOp>(
                    loc,
                    rowOffset,
                    col
                );

            auto elemPtr =
                builder->create<mlir::LLVM::GEPOp>(
                    loc,
                    mlir::LLVM::LLVMPointerType::get(ctx),
                    matrixTy,
                    arrayPtr,
                    mlir::ValueRange{
                        i32(0),
                        linearIndex
                    }
                );

            auto elem =
                builder->create<mlir::LLVM::LoadOp>(
                    loc,
                    elemTy,
                    elemPtr
                );

            printInline(elem);

            emitIfNotLast(col, colCount, [&] {
                emitSeparator();
            });
        });

        emitChar(']');

        emitIfNotLast(row, rowCount, [&] {
            emitSeparator();
        });
    });

    emitChar(']');
}

/// =============================================================
/// Indexed Printing
/// =============================================================

void PrintHelper::printIndexed(
    mlir::Value value,
    std::shared_ptr<IndexExpr> idx
) {
    auto resultTy = idx->type;

    if (resultTy->kind == TypeKind::Primitive) {

        auto loaded =
            builder->create<mlir::LLVM::LoadOp>(
                loc,
                types->getMLIRType(resultTy),
                value
            );

        printInline(loaded);
        return;
    }

    if (resultTy->kind == TypeKind::Array) {

        auto arrType =
            std::static_pointer_cast<ArrayType>(
                resultTy
            );

        // matrix[row] -> row slice
        if (arrType->isMatrix()) {

            auto rowType =
                std::make_shared<ArrayType>(
                    arrType->elem,
                    arrType->dimensions().second
                );

            printFlatArray(value, rowType);
            return;
        }

        printFlatArray(value, arrType);
        return;
    }

    llvm::report_fatal_error(
        "Unsupported indexed type in printIndexed"
    );
}

/// =============================================================
/// Tuples
/// =============================================================

void PrintHelper::printTuple(
    mlir::Value tupPtr,
    mlir::LLVM::LLVMStructType structTy,
    std::vector<std::shared_ptr<Type>> fieldTypes
) {

    auto ptrTy =
        mlir::LLVM::LLVMPointerType::get(
            builder->getContext()
        );

    emitChar('(');

    for (size_t i = 0; i < fieldTypes.size(); ++i) {

        auto fieldPtr =
            builder->create<mlir::LLVM::GEPOp>(
                loc,
                ptrTy,
                structTy,
                tupPtr,
                mlir::ArrayRef<mlir::LLVM::GEPArg>{
                    mlir::LLVM::GEPArg(0),
                    mlir::LLVM::GEPArg(
                        static_cast<int64_t>(i)
                    )
                }
            );

        auto fieldTy =
            structTy.getBody()[i];

        auto value =
            builder->create<mlir::LLVM::LoadOp>(
                loc,
                fieldTy,
                fieldPtr
            );

        printValue(value, fieldTypes[i]);

        if (i != fieldTypes.size() - 1) {
            emitSeparator();
        }
    }

    emitChar(')');
}

} // namespace rex