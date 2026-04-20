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

        // // newline AFTER full print
        // auto fmtChar = getFmtAddress(fmt_char);
        // auto nl = builder->create<mlir::arith::ConstantIntOp>(loc, '\n', 8);

        // builder->create<mlir::LLVM::CallOp>(
        //     loc,
        //     printf_func,
        //     mlir::ValueRange{fmtChar, nl}
        // );
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
    void PrintHelper::printArray(mlir::Value arrayPtr, std::shared_ptr<ArrayType> arrType) {
        auto ctx = builder->getContext();

        auto i32Ty = builder->getI32Type();
        auto zero  = builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32);
        auto one   = builder->create<mlir::arith::ConstantIntOp>(loc, 1, 32);
        auto sizeC = builder->create<mlir::arith::ConstantIntOp>(loc, arrType->size, 32);

        auto ptrTy = mlir::LLVM::LLVMPointerType::get(ctx);

        // -------------------------
        // print "["
        // -------------------------
        auto fmtChar = getFmtAddress(fmt_char);
        auto open = builder->create<mlir::arith::ConstantIntOp>(loc, '[', 8);

        builder->create<mlir::LLVM::CallOp>(
            loc, printf_func, mlir::ValueRange{fmtChar, open}
        );

        // -------------------------
        // LOOP
        // -------------------------
        auto loop = builder->create<mlir::scf::ForOp>(loc, zero, sizeC, one);
        builder->setInsertionPointToStart(loop.getBody());

        auto iv = loop.getInductionVar();

        // -------------------------
        // 🔥 FIX: element type
        // -------------------------
        mlir::Type elemTy;

        if (arrType->elem->kind == TypeKind::Array) {
            // nested array → pointer to sub-array
            elemTy = ptrTy;
        } else {
            elemTy = types->getMLIRType(arrType->elem);
        }

        // -------------------------
        // GEP
        // -------------------------
        auto elemPtr = builder->create<mlir::LLVM::GEPOp>(
            loc,
            ptrTy,
            elemTy,
            arrayPtr,
            mlir::ValueRange{iv}
        );

        // -------------------------
        // LOAD
        // -------------------------
        auto elemVal = builder->create<mlir::LLVM::LoadOp>(
            loc,
            elemTy,
            elemPtr
        );

        // -------------------------
        // PRINT (recursive if needed)
        // -------------------------
        if (arrType->elem->kind == TypeKind::Array) {
            printArray(
                elemVal,
                std::static_pointer_cast<ArrayType>(arrType->elem)
            );
        } else {
            printInline(elemVal);
        }

        // -------------------------
        // ", " logic
        // -------------------------
        auto lastIdx = builder->create<mlir::arith::SubIOp>(loc, sizeC, one);

        auto cond = builder->create<mlir::arith::CmpIOp>(
            loc,
            mlir::arith::CmpIPredicate::ne,
            iv,
            lastIdx
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
    }
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