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

 mlir::LLVM::AddressOfOp PrintHelper::getFmtAddress(mlir::LLVM::GlobalOp fmt){
    return builder->create<mlir::LLVM::AddressOfOp>(loc,fmt);
 }

void PrintHelper::printPrimtive(mlir::Value val) {
    printInline(val);

    // newline AFTER full print
    auto fmtChar = getFmtAddress(fmt_char);
    auto nl = builder->create<mlir::arith::ConstantIntOp>(loc, '\n', 8);

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, nl}
    );
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

 void PrintHelper::printString(mlir::LLVM::GlobalOp val){
    // Get the format string "%s"
    auto fmt = getFmtAddress(fmt_string);

    // Get the pointer to the global string
    mlir::Value strPtr = builder->create<mlir::LLVM::AddressOfOp>(loc, val);

    // Call printf("%s", strPtr)
    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmt, strPtr}
    );

    // Print a newline after the string
    auto fmtChar = getFmtAddress(fmt_char);
    auto nl = builder->create<mlir::arith::ConstantIntOp>(loc, '\n', 8);

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, nl}
    );
}

    void PrintHelper::printArray(mlir::Value arrayPtr, std::shared_ptr<ArrayType> arrType) {
        auto i32Ty = builder->getI32Type();
        auto zero  = builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32);
        auto one   = builder->create<mlir::arith::ConstantIntOp>(loc, 1, 32);
        auto sizeC = builder->create<mlir::arith::ConstantIntOp>(loc, arrType->size, 32);

        // -------------------------
        // print "["
        // -------------------------
        auto fmtChar = getFmtAddress(fmt_char);
        auto open = builder->create<mlir::arith::ConstantIntOp>(loc, '[', 8);

        builder->create<mlir::LLVM::CallOp>(
            loc, printf_func, mlir::ValueRange{fmtChar, open}
        );

        // -------------------------
        // loop setup
        // -------------------------
        auto func = builder->getBlock()->getParentOp();
        auto loop = builder->create<mlir::scf::ForOp>(loc, zero, sizeC, one);

        builder->setInsertionPointToStart(loop.getBody());

        auto iv = loop.getInductionVar();

        // -------------------------
        // element type (IMPORTANT)
        // -------------------------
        auto elemTy = types->getMLIRType(arrType->elem);

        // -------------------------
        // GEP: ptr[i]
        // -------------------------
        auto elemPtr = builder->create<mlir::LLVM::GEPOp>(
            loc,
            mlir::LLVM::LLVMPointerType::get(builder->getContext()),
            elemTy,
            arrayPtr,
            mlir::ValueRange{iv}
        );

        // -------------------------
        // LOAD element
        // -------------------------
        auto elemVal = builder->create<mlir::LLVM::LoadOp>(
            loc,
            elemTy,
            elemPtr
        );

        // -------------------------
        // print element
        // -------------------------
        printInline(elemVal);

        // -------------------------
        // print ", " if not last
        // -------------------------
        auto cond = builder->create<mlir::arith::CmpIOp>(
            loc,
            mlir::arith::CmpIPredicate::ne,
            iv,
            builder->create<mlir::arith::SubIOp>(loc, sizeC, one)
        );

        auto ifOp = builder->create<mlir::scf::IfOp>(loc, cond, false);

        builder->setInsertionPointToStart(&ifOp.getThenRegion().front());

        auto comma = builder->create<mlir::arith::ConstantIntOp>(loc, ',', 8);
        auto space = builder->create<mlir::arith::ConstantIntOp>(loc, ' ', 8);

        builder->create<mlir::LLVM::CallOp>(
            loc, printf_func, mlir::ValueRange{fmtChar, comma}
        );

        builder->create<mlir::LLVM::CallOp>(
            loc, printf_func, mlir::ValueRange{fmtChar, space}
        );

        builder->setInsertionPointAfter(ifOp);

        // -------------------------
        // after loop → print "]"
        // -------------------------
        builder->setInsertionPointAfter(loop);

        auto close = builder->create<mlir::arith::ConstantIntOp>(loc, ']', 8);

        builder->create<mlir::LLVM::CallOp>(
            loc, printf_func, mlir::ValueRange{fmtChar, close}
        );

        // newline
        auto nl = builder->create<mlir::arith::ConstantIntOp>(loc, '\n', 8);

        builder->create<mlir::LLVM::CallOp>(
            loc, printf_func, mlir::ValueRange{fmtChar, nl}
        );
    }
}