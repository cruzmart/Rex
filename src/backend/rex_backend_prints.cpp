#include "backend/rex_backend_prints.h"
#include <llvm/Support/Error.h>
#include <mlir/Dialect/LLVMIR/LLVMDialect.h>

PrintHelper::PrintHelper(
                         mlir::OpBuilder &b,
                         mlir::Location l)
    :  builder(&b), loc(l) {
    }

 mlir::LLVM::AddressOfOp PrintHelper::getFmtAddress(mlir::LLVM::GlobalOp fmt){
    return builder->create<mlir::LLVM::AddressOfOp>(loc,fmt);
 }

 void PrintHelper::printPrimtive(mlir::Value val) {
    mlir::Type type = val.getType();
    mlir::Value fmt;

    auto fmtChar = getFmtAddress(fmt_char);
    auto nl = builder->create<mlir::arith::ConstantIntOp>(loc, '\n', 8);

    if (type.isInteger(32)) {
        fmt = getFmtAddress(fmt_int);
    } else if (type.isInteger(8)) {
        fmt = getFmtAddress(fmt_char);
    } else if (type.isInteger(1)) {
        // bool -> extend to i32 for printf
        val = builder->create<mlir::arith::ExtUIOp>(loc, builder->getI32Type(), val);
        fmt = getFmtAddress(fmt_int);
    } else if (type.isF32()) {
        // extend float to double
        val = builder->create<mlir::arith::ExtFOp>(loc, builder->getF64Type(), val);
        fmt = getFmtAddress(fmt_float);

    }  else {
        llvm::errs() << "Unsupported type for printValue\n";
        return;
    }

    // Call printf

    mlir::ValueRange args = {fmt, val};
    builder->create<mlir::LLVM::CallOp>(loc, printf_func, args);

    builder->create<mlir::LLVM::CallOp>(
        loc,
        printf_func,
        mlir::ValueRange{fmtChar, nl}
    );

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