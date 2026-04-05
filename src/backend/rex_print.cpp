#include "backend/rex_print.h"

PrintHelper::PrintHelper(mlir::MLIRContext &ctx,
                         mlir::OpBuilder &b,
                         mlir::Location &l)
    : context(ctx), builder(b), loc(l) {}



 mlir::LLVM::AddressOfOp PrintHelper::getFmtAddress(mlir::LLVM::GlobalOp fmt){
    return builder.create<mlir::LLVM::AddressOfOp>(loc,fmt);
 }

 void PrintHelper::print(mlir::Value val) {
    mlir::Type type = val.getType();
    mlir::Value fmt;

    if (type.isInteger(32)) {
        fmt = getFmtAddress(fmt_int);
    } else if (type.isInteger(8)) {
        fmt = getFmtAddress(fmt_char);
    } else if (type.isInteger(1)) {
        // bool -> extend to i32 for printf
        val = builder.create<mlir::arith::ExtUIOp>(loc, builder.getI32Type(), val);
        fmt = getFmtAddress(fmt_int);
    } else if (type.isF32()) {
        // extend float to double
        val = builder.create<mlir::arith::ExtFOp>(loc, builder.getF64Type(), val);
        fmt = getFmtAddress(fmt_float);
    } else if (auto ptr = type.dyn_cast<mlir::LLVM::LLVMPointerType>()) {
        fmt = getFmtAddress(fmt_string);
    } else {
        llvm::errs() << "Unsupported type for printValue\n";
        return;
    }

    // Call printf
    // builder.create<mlir::LLVM::CallOp>(
    //     loc,
    //     mlir::TypeRange{},
    //     mlir::SymbolRefAttr::get(&context, "printf"),
    //     mlir::ValueRange{fmt, val}
    // );

    mlir::ValueRange args = {fmt, val};
    builder.create<mlir::LLVM::CallOp>(loc, printf_func, args);
}