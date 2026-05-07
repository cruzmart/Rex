#include "backend/rex_backend_prints.h"
#include "backend/rex_backend_types.h"
#include <llvm/Support/Error.h>
#include <mlir/Dialect/LLVMIR/LLVMDialect.h>
#include <mlir/Dialect/LLVMIR/LLVMTypes.h>


namespace rex {

PrintHelper::PrintHelper(
                         std::shared_ptr<mlir::OpBuilder> b,
                         mlir::Location l,
                        std::shared_ptr<TypesHelper> t)
    :  builder(b), loc(l), types(t) {
    }

/// =============================================================
/// ALiteral Printing Function + Helpers
/// =============================================================

mlir::LLVM::AddressOfOp PrintHelper::getFmtAddress(mlir::LLVM::GlobalOp fmt){
    return builder->create<mlir::LLVM::AddressOfOp>(loc,fmt);
}
void PrintHelper::printInline(mlir::Value val) {
    mlir::Type type = val.getType();
    mlir::Value fmt;

    if (type.isInteger(32)) {
        fmt = getFmtAddress(fmt_int);
    } else if (type.isInteger(8)) {
        fmt = getFmtAddress(fmt_char);
    } else if (type.isInteger(1)) {
        val = builder->create<mlir::arith::ExtUIOp>(loc, builder->getI32Type(), val);
        fmt = getFmtAddress(fmt_int);
    } else if (type.isF32()) {
        val = builder->create<mlir::arith::ExtFOp>(loc, builder->getF64Type(), val);
        fmt = getFmtAddress(fmt_float);
    } else if (type.isa<mlir::LLVM::LLVMPointerType>()) {
        fmt = getFmtAddress(fmt_string);
    } else {
        llvm::report_fatal_error("Unsupported type in printInline");
    }

    builder->create<mlir::LLVM::CallOp>(loc, printf_func, mlir::ValueRange{fmt, val});
}



/// =============================================================
/// Array/Matrix Printing Function + Helpers
/// =============================================================

void PrintHelper::printArray(
    mlir::Value arrayPtr,
    std::shared_ptr<ArrayType> arrType
) {
   if (arrType->isMatrix()) {
        printMatrix(arrayPtr, arrType);
    } else {
        printFlatArray(arrayPtr, arrType);
    }
}

void PrintHelper::printFlatArray(mlir::Value arrayPtr, std::shared_ptr<ArrayType> arrType) {
    auto ctx = builder->getContext();

    auto zero = builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32);
    auto one  = builder->create<mlir::arith::ConstantIntOp>(loc, 1, 32);

    auto elemTy = types->getMLIRType(arrType->elem);

    auto [rows, cols] = arrType->dimensions();

    int total = rows * cols;

    auto size = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        total,
        32
    );

    auto fmtChar = getFmtAddress(fmt_char);

    auto arrayTy = mlir::LLVM::LLVMArrayType::get(elemTy, total);

    // =====================================================
    // '['
    // =====================================================

    auto open = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        '[',
        8
    );

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, open}
    );

    // =====================================================
    // LOOP
    // =====================================================

    auto loop = builder->create<mlir::scf::ForOp>(
        loc,
        zero,
        size,
        one
    );

    builder->setInsertionPointToStart(loop.getBody());

    auto i = loop.getInductionVar();

    auto elemPtr = builder->create<mlir::LLVM::GEPOp>(
        loc,
        mlir::LLVM::LLVMPointerType::get(ctx),
        arrayTy,
        arrayPtr,
        mlir::ValueRange{zero, i}
    );

    auto elemVal = builder->create<mlir::LLVM::LoadOp>(
        loc,
        elemTy,
        elemPtr
    );

    printInline(elemVal);

    // =====================================================
    // ", "
    // =====================================================

    auto lastIndex = builder->create<mlir::arith::SubIOp>(
        loc,
        size,
        one
    );

    auto isNotLast = builder->create<mlir::arith::CmpIOp>(
        loc,
        mlir::arith::CmpIPredicate::ne,
        i,
        lastIndex
    );

    auto sepIf = builder->create<mlir::scf::IfOp>(
        loc,
        isNotLast,
        false
    );

    builder->setInsertionPointToStart(
        &sepIf.getThenRegion().front()
    );

    auto comma = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        ',',
        8
    );

    auto space = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        ' ',
        8
    );

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, comma}
    );

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, space}
    );

    builder->setInsertionPointAfter(sepIf);

    builder->setInsertionPointAfter(loop);

    // =====================================================
    // ']'
    // =====================================================

    auto close = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        ']',
        8
    );

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, close}
    );
}
void PrintHelper::printMatrix(mlir::Value arrayPtr, std::shared_ptr<ArrayType> arrType) {
    auto ctx = builder->getContext();

    auto zero = builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32);
    auto one  = builder->create<mlir::arith::ConstantIntOp>(loc, 1, 32);

    auto elemTy = types->getMLIRType(arrType->elem);

    auto [rows, cols] = arrType->dimensions();

    auto rowSize = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        rows,
        32
    );

    auto colSize = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        cols,
        32
    );

    auto total = rows * cols;

    auto arrayTy = mlir::LLVM::LLVMArrayType::get(
        elemTy,
        total
    );

    auto fmtChar = getFmtAddress(fmt_char);

    // =====================================================
    // OUTER '['
    // =====================================================

    auto open = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        '[',
        8
    );

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, open}
    );

    // =====================================================
    // ROW LOOP
    // =====================================================

    auto rowLoop = builder->create<mlir::scf::ForOp>(
        loc,
        zero,
        rowSize,
        one
    );

    builder->setInsertionPointToStart(rowLoop.getBody());

    auto row = rowLoop.getInductionVar();

    // '['
    auto rowOpen = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        '[',
        8
    );

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, rowOpen}
    );

    // =====================================================
    // COLUMN LOOP
    // =====================================================

    auto colLoop = builder->create<mlir::scf::ForOp>(
        loc,
        zero,
        colSize,
        one
    );

    builder->setInsertionPointToStart(colLoop.getBody());

    auto col = colLoop.getInductionVar();

    // linearIndex = row * cols + col

    auto colsConst = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        cols,
        32
    );

    auto rowOffset = builder->create<mlir::arith::MulIOp>(
        loc,
        row,
        colsConst
    );

    auto linearIndex = builder->create<mlir::arith::AddIOp>(
        loc,
        rowOffset,
        col
    );

    auto elemPtr = builder->create<mlir::LLVM::GEPOp>(
        loc,
        mlir::LLVM::LLVMPointerType::get(ctx),
        arrayTy,
        arrayPtr,
        mlir::ValueRange{zero, linearIndex}
    );

    auto elemVal = builder->create<mlir::LLVM::LoadOp>(
        loc,
        elemTy,
        elemPtr
    );

    printInline(elemVal);

    // =====================================================
    // COLUMN ", "
    // =====================================================

    auto lastCol = builder->create<mlir::arith::SubIOp>(
        loc,
        colSize,
        one
    );

    auto notLastCol = builder->create<mlir::arith::CmpIOp>(
        loc,
        mlir::arith::CmpIPredicate::ne,
        col,
        lastCol
    );

    auto colSepIf = builder->create<mlir::scf::IfOp>(
        loc,
        notLastCol,
        false
    );

    builder->setInsertionPointToStart(
        &colSepIf.getThenRegion().front()
    );

    auto comma = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        ',',
        8
    );

    auto space = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        ' ',
        8
    );

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, comma}
    );

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, space}
    );

    builder->setInsertionPointAfter(colSepIf);

    builder->setInsertionPointAfter(colLoop);

    // =====================================================
    // ']'
    // =====================================================

    auto rowClose = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        ']',
        8
    );

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, rowClose}
    );

    // =====================================================
    // ROW ", "
    // =====================================================

    auto lastRow = builder->create<mlir::arith::SubIOp>(
        loc,
        rowSize,
        one
    );

    auto notLastRow = builder->create<mlir::arith::CmpIOp>(
        loc,
        mlir::arith::CmpIPredicate::ne,
        row,
        lastRow
    );

    auto rowSepIf = builder->create<mlir::scf::IfOp>(
        loc,
        notLastRow,
        false
    );

    builder->setInsertionPointToStart(
        &rowSepIf.getThenRegion().front()
    );

    auto comma2 = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        ',',
        8
    );

    auto space2 = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        ' ',
        8
    );

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, comma2}
    );

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, space2}
    );

    builder->setInsertionPointAfter(rowSepIf);

    builder->setInsertionPointAfter(rowLoop);

    // =====================================================
    // OUTER ']'
    // =====================================================

    auto close = builder->create<mlir::arith::ConstantIntOp>(
        loc,
        ']',
        8
    );

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, close}
    );
}



/// =============================================================
/// Tuple Printing Function
/// =============================================================
void PrintHelper::printTuple(mlir::Value tupPtr, mlir::LLVM::LLVMStructType t_s, std::vector<std::shared_ptr<Type>> t){


    auto ptrTy = mlir::LLVM::LLVMPointerType::get(builder->getContext());
    auto structTy = t_s;
    auto comma = builder->create<mlir::arith::ConstantIntOp>(loc, ',', 8);
    auto space = builder->create<mlir::arith::ConstantIntOp>(loc, ' ', 8);

    // -----------------------------------
    // print '('
    // -----------------------------------
    auto fmtChar = getFmtAddress(fmt_char);

    auto open = builder->create<mlir::arith::ConstantIntOp>(loc, '(', 8);
    builder->create<mlir::LLVM::CallOp>(
        loc, printf_func, mlir::ValueRange{fmtChar, open}
    );

    // -----------------------------------
    // loop fields
    // -----------------------------------
    for (size_t i = 0; i < t.size(); i++) {

        // -----------------------------------
        // GEP → field pointer
        // -----------------------------------
        auto fieldPtr = builder->create<mlir::LLVM::GEPOp>(
            loc,
            ptrTy,
            structTy,
            tupPtr,
            mlir::ArrayRef<mlir::LLVM::GEPArg>{
                mlir::LLVM::GEPArg(0),
                mlir::LLVM::GEPArg((int64_t)i)
            }
        );

        // -----------------------------------
        // LOAD field
        // -----------------------------------
        mlir::Type fieldTy = t_s.getBody()[i];

        auto value = builder->create<mlir::LLVM::LoadOp>(
            loc,
            fieldTy,
            fieldPtr
        );

        // -----------------------------------
        // PRINT value
        // -----------------------------------
        if (t[i]->kind == TypeKind::Primitive) {
            auto prim = std::static_pointer_cast<PrimType>(t[i]);

            switch (prim->prim) {
                case PrimType::Prims::Int:
                case PrimType::Prims::Real:
                case PrimType::Prims::Bool:
                case PrimType::Prims::Char:
                case PrimType::Prims::String:
                    printInline(value);
                    break;
                default:
                    llvm::report_fatal_error("Type unknown to be printed");

            }
        }
        else if (t[i]->kind == TypeKind::Array) {
            printArray(value, std::static_pointer_cast<ArrayType>(t[i]));
        }

        // -----------------------------------
        // print ", " if not last
        // -----------------------------------
        if (i != t.size() - 1) {

            builder->create<mlir::LLVM::CallOp>(
                loc, printf_func, mlir::ValueRange{fmtChar, comma}
            );
            builder->create<mlir::LLVM::CallOp>(
                loc, printf_func, mlir::ValueRange{fmtChar, space}
            );
        }
    }

    // -----------------------------------
    // print ')'
    // -----------------------------------
    auto close = builder->create<mlir::arith::ConstantIntOp>(loc, ')', 8);

    builder->create<mlir::LLVM::CallOp>(
        loc, printf_func, mlir::ValueRange{fmtChar, close}
    );
}
}