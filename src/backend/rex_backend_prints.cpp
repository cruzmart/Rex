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
    return builder->create<mlir::LLVM::AddressOfOp>(
        loc,
        fmt
    );
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

mlir::Type PrintHelper::ptrTy() {
    return mlir::LLVM::LLVMPointerType::get(
        builder->getContext()
    );
}

/// ------------------------------------------------------------
/// Generic GEP helper
/// ------------------------------------------------------------

mlir::Value PrintHelper::gep(
    mlir::Type elemTy,
    mlir::Value basePtr,
    mlir::ValueRange indices
) {
    return builder->create<mlir::LLVM::GEPOp>(
        loc,
        ptrTy(),
        elemTy,
        basePtr,
        indices
    );
}

/// ------------------------------------------------------------
/// Load helper
/// ------------------------------------------------------------

mlir::Value PrintHelper::load(
    mlir::Type type,
    mlir::Value ptr
) {
    return builder->create<mlir::LLVM::LoadOp>(
        loc,
        type,
        ptr
    );
}

/// ------------------------------------------------------------
/// Array element load
/// ------------------------------------------------------------

mlir::Value PrintHelper::loadArrayElem(
    mlir::Value arrayPtr,
    mlir::Type elemTy,
    mlir::Value index
) {
    auto elemPtr =
        gep(
            elemTy,
            arrayPtr,
            mlir::ValueRange{index}
        );

    return load(elemTy, elemPtr);
}

/// ------------------------------------------------------------
/// Printf
/// ------------------------------------------------------------

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
    emitPrintf(
        getFmtAddress(fmt_char),
        i8(c)
    );
}

void PrintHelper::emitSeparator() {
    emitChar(',');
    emitChar(' ');
}

/// ------------------------------------------------------------
/// Generic For Loop
/// ------------------------------------------------------------

template<typename Fn>
void PrintHelper::forLoop(
    mlir::Value upperBound,
    Fn &&body
) {
    auto loop =
        builder->create<mlir::scf::ForOp>(
            loc,
            i32(0),
            upperBound,
            i32(1)
        );

    builder->setInsertionPointToStart(
        loop.getBody()
    );

    body(loop.getInductionVar());

    builder->setInsertionPointAfter(loop);
}

/// ------------------------------------------------------------
/// Emit if not last index
/// ------------------------------------------------------------

template<typename Fn>
void PrintHelper::emitIfNotLast(
    mlir::Value index,
    mlir::Value size,
    Fn &&body
) {
    auto last =
        builder->create<mlir::arith::SubIOp>(
            loc,
            size,
            i32(1)
        );

    auto cond =
        builder->create<mlir::arith::CmpIOp>(
            loc,
            mlir::arith::CmpIPredicate::ne,
            index,
            last
        );

    auto ifOp =
        builder->create<mlir::scf::IfOp>(
            loc,
            cond,
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

void PrintHelper::printInline(
    mlir::Value value
) {

    auto type = value.getType();

    mlir::Value fmt;

    // bool
    if (type.isInteger(1)) {

        value =
            builder->create<mlir::arith::ExtUIOp>(
                loc,
                builder->getI32Type(),
                value
            );

        fmt = getFmtAddress(fmt_int);
    }

    // int
    else if (type.isInteger(32)) {
        fmt = getFmtAddress(fmt_int);
    }

    // char
    else if (type.isInteger(8)) {
        fmt = getFmtAddress(fmt_char);
    }

    // float
    else if (type.isF32()) {

        value =
            builder->create<mlir::arith::ExtFOp>(
                loc,
                builder->getF64Type(),
                value
            );

        fmt = getFmtAddress(fmt_float);
    }

    // string
    else if (
        type.isa<mlir::LLVM::LLVMPointerType>()
    ) {
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
/// Generic Dispatcher
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
                std::static_pointer_cast<ArrayType>(
                    type
                )
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

    auto elemTy =
        this->types->getMLIRType(arrType->elem);

    auto [rows, cols] =
        arrType->dimensions();

    auto totalSize =
        i32(rows * cols);

    emitChar('[');

    forLoop(totalSize, [&](mlir::Value i) {

        auto elem =
            loadArrayElem(
                arrayPtr,
                elemTy,
                i
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

    auto [rows, cols] =
        arrType->dimensions();

    auto rowCount = i32(rows);
    auto colCount = i32(cols);

    auto matrixTy =
        this->types->getMLIRType(arrType);

    auto elemTy =
        this->types->getMLIRType(
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
                gep(
                    matrixTy,
                    arrayPtr,
                    mlir::ValueRange{
                        i32(0),
                        linearIndex
                    }
                );

            auto elem =
                load(elemTy, elemPtr);

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

void PrintHelper::printIndexed(
    mlir::Value value,
    std::shared_ptr<IndexExpr> idx
) {
    auto resultTy = idx->type;

    // =========================================================
    // Primitive index result
    // =========================================================
    if (resultTy->kind == TypeKind::Primitive) {

        auto loadedValue =
            builder->create<mlir::LLVM::LoadOp>(
                loc,
                this->types->getMLIRType(resultTy),
                value
            );

        printInline(loadedValue);
        return;
    }

    // =========================================================
    // Array / Matrix index result
    // =========================================================
    if (resultTy->kind == TypeKind::Array) {

        auto arrType =
            std::static_pointer_cast<ArrayType>(resultTy);

        // matrix[row] → slice view
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
    const std::vector<std::shared_ptr<Type>>& fieldTypes
) {
    auto ptrTy = this->types->ptrty();

    emitChar('(');

    for (size_t i = 0; i < fieldTypes.size(); ++i) {

        // =====================================================
        // Field pointer (struct access)
        // =====================================================
        auto fieldPtr =
            builder->create<mlir::LLVM::GEPOp>(
                loc,
                ptrTy,
                structTy,
                tupPtr,
                mlir::ArrayRef<mlir::LLVM::GEPArg>{
                    mlir::LLVM::GEPArg(0),
                    mlir::LLVM::GEPArg(static_cast<int32_t>(i))
                }
            );

        auto fieldTy =
            structTy.getBody()[i];

        auto fieldValue =
            builder->create<mlir::LLVM::LoadOp>(
                loc,
                fieldTy,
                fieldPtr
            );

        printValue(fieldValue, fieldTypes[i]);

        // =====================================================
        // Separator logic
        // =====================================================
        if (i + 1 < fieldTypes.size()) {
            emitSeparator();
        }
    }

    emitChar(')');
}

} // namespace rex