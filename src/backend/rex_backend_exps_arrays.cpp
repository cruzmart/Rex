
#include "backend/rex_backend_exps.h"
#include "rex_exps.h"
#include "rex_ops.h"
#include "rex_types.h"
#include <memory>
#include <mlir/Conversion/ArithToLLVM/ArithToLLVM.h>
#include <mlir/Dialect/Arith/IR/Arith.h>
#include <mlir/Dialect/LLVMIR/LLVMDialect.h>
#include <mlir/Dialect/LLVMIR/LLVMTypes.h>
#include <mlir/IR/Builders.h>
#include <mlir/IR/BuiltinAttributes.h>
#include <mlir/IR/ValueRange.h>

namespace rex {


                                        /// =============================================================
                                        ///                     Array Handling
                                        /// =============================================================

    /// Checks if array expression is fully constant.
    /// Allows:
    ///  - literals
    ///  - nested arrays
    ///  - constant string expressions

    /// =========================================================
    /// Vector <op> Scalar
    /// =========================================================
    mlir::Value ExpressionsHelper::createVectorScalarOp(
        mlir::Value lhs,
        std::shared_ptr<Type> lhs_t,
        mlir::Value rhs,
        std::shared_ptr<Type> rhs_t,
        BinaryOp op,
        bool vectorIsLHS,
        std::shared_ptr<ArrayType> res_t
    ) {

        // =====================================================
        // NORMALIZE OPERANDS
        // =====================================================

        mlir::Value vectorVal =
            vectorIsLHS ? lhs : rhs;

        mlir::Value scalarVal =
            vectorIsLHS ? rhs : lhs;

        auto vectorTy =
            vectorIsLHS
                ? cast<ArrayType>(lhs_t)
                : cast<ArrayType>(rhs_t);

        auto scalarTy =
            vectorIsLHS
                ? cast<PrimType>(rhs_t)
                : cast<PrimType>(lhs_t);

        (void)scalarTy;

        // =====================================================
        // DIMENSIONS
        // =====================================================

        auto [rows, cols] =
            vectorTy->dimensions();

        int totalSize =
            rows * cols;

        auto zero = i32(0);
        auto one  = i32(1);
        auto size = i32(totalSize);

        // =====================================================
        // TYPES
        // =====================================================

        auto ptrTy =
            ptrty();

        auto vectorElemTy =
            this->types->getMLIRType(
                vectorTy->elem
            );

        auto resultPrimTy =
            cast<PrimType>(res_t->elem);

        auto resultElemTy =
            this->types->getMLIRType(
                res_t->elem
            );

        auto resultArrayTy =
            mlir::LLVM::LLVMArrayType::get(
                resultElemTy,
                totalSize
            );

        // =====================================================
        // RESULT STORAGE
        // =====================================================

        auto resultPtr =
            builder->create<mlir::LLVM::AllocaOp>(
                loc,
                ptrTy,
                resultArrayTy,
                one
            );

        // =====================================================
        // LOOP
        // =====================================================

        auto loop =
            builder->create<mlir::scf::ForOp>(
                loc,
                zero,
                size,
                one
            );

        builder->setInsertionPointToStart(
            loop.getBody()
        );

        {
            auto i =
                loop.getInductionVar();

            // -------------------------------------------------
            // VECTOR ELEMENT
            // -------------------------------------------------

            auto vectorElem =
                loadArrayElem(
                    vectorVal,
                    vectorElemTy,
                    i
                );

            // -------------------------------------------------
            // ORDERED OPERANDS
            // -------------------------------------------------

            mlir::Value lhsValue =
                vectorIsLHS
                    ? vectorElem
                    : scalarVal;

            mlir::Value rhsValue =
                vectorIsLHS
                    ? scalarVal
                    : vectorElem;

            // -------------------------------------------------
            // OPERATION
            // -------------------------------------------------

            auto resultValue =
                createBinaryExp(
                    lhsValue,
                    rhsValue,
                    resultPrimTy->prim,
                    op
                );

            // -------------------------------------------------
            // STORE RESULT
            // -------------------------------------------------

            storeArrayElem(
                resultPtr,
                resultElemTy,
                i,
                resultValue
            );
        }

        builder->setInsertionPointAfter(loop);

        return resultPtr;
    }

    mlir::Value ExpressionsHelper::createVectorVectorOp(
        mlir::Value lhs,
        std::shared_ptr<ArrayType> lhs_t,
        mlir::Value rhs,
        std::shared_ptr<ArrayType> rhs_t,
        BinaryOp op,
        std::shared_ptr<ArrayType> res_t
    ) {

        auto ptrTy = ptrty();

        auto zero = 
            i32(0);
        
        auto one =
            i32(1);

        // =====================================================
        // DIMENSIONS
        // =====================================================

        auto [lhsRows, lhsCols] = lhs_t->dimensions();
        auto [rhsRows, rhsCols] = rhs_t->dimensions();

        int lhsSize = lhsRows * lhsCols;
        int rhsSize = rhsRows * rhsCols;
        int resultSize = std::max(lhsSize, rhsSize);

        auto lhsSizeV = 
            i32(lhsSize);
            
        auto rhsSizeV = 
            i32(rhsSize);
        
        auto resultSizeV = 
            i32(resultSize);

        // =====================================================
        // TYPES
        // =====================================================

        auto lhsElemTy =
            this->types->getMLIRType(cast<PrimType>(lhs_t->elem));

        auto rhsElemTy =
            this->types->getMLIRType(cast<PrimType>(rhs_t->elem));

        auto resultElemTy =
            this->types->getMLIRType(cast<PrimType>(res_t->elem));

        auto resultPrimTy =
            cast<PrimType>(res_t->elem);

        // =====================================================
        // RESULT ARRAY
        // =====================================================

        auto resultArrayTy =
            mlir::LLVM::LLVMArrayType::get(resultElemTy, resultSize);
            

        auto resultPtr =
            builder->create<mlir::LLVM::AllocaOp>(
                loc, ptrTy, resultArrayTy, one
            );

        auto paddingValue = getOpDefault(op);

        // =====================================================
        // PAD RESULT ARRAY
        // =====================================================

        auto padLoop =
            builder->create<mlir::scf::ForOp>(loc, zero, resultSizeV, one);

        builder->setInsertionPointToStart(padLoop.getBody());
        {
            auto i = padLoop.getInductionVar();

            storeArrayElem(
                resultPtr,
                resultElemTy,
                i,
                paddingValue
            );
        }
        builder->setInsertionPointAfter(padLoop);

        // =====================================================
        // COPY LHS INTO RESULT
        // =====================================================

        auto lhsLoop =
            builder->create<mlir::scf::ForOp>(loc, zero, lhsSizeV, one);

        builder->setInsertionPointToStart(lhsLoop.getBody());
        {
            auto i = lhsLoop.getInductionVar();

            auto lhsElem =
                loadArrayElem(lhs, lhsElemTy, i);

            storeArrayElem(
                resultPtr,
                resultElemTy,
                i,
                lhsElem
            );
        }
        builder->setInsertionPointAfter(lhsLoop);

        // =====================================================
        // APPLY RHS OPERATION
        // =====================================================

        auto rhsLoop =
            builder->create<mlir::scf::ForOp>(loc, zero, rhsSizeV, one);

        builder->setInsertionPointToStart(rhsLoop.getBody());
        {
            auto i = rhsLoop.getInductionVar();

            auto resultElem =
                loadArrayElem(resultPtr, resultElemTy, i);

            auto rhsElem =
                loadArrayElem(rhs, rhsElemTy, i);

            auto newValue =
                createBinaryExp(
                    resultElem,
                    rhsElem,
                    resultPrimTy->prim,
                    op
                );

            storeArrayElem(
                resultPtr,
                resultElemTy,
                i,
                newValue
            );
        }
        builder->setInsertionPointAfter(rhsLoop);

        return resultPtr;
    }

    mlir::Value ExpressionsHelper::createRuntimeArray(
        const std::vector<mlir::Value>& elements,
        std::shared_ptr<PrimType> kind
    ) {
        mlir::Type elemTy = this->types->getMLIRType(kind);

        auto ctx = builder->getContext();

        auto ptrTy = mlir::LLVM::LLVMPointerType::get(ctx);

        // =====================================================
        // ✅ REAL LLVM ARRAY TYPE
        // =====================================================
        auto arrayTy = mlir::LLVM::LLVMArrayType::get(elemTy, elements.size());

        // =====================================================
        // ✅ ALLOCATE ONE ARRAY OBJECT
        // =====================================================
        auto one = builder->create<mlir::arith::ConstantIntOp>(loc, 1, 32);

        auto alloca = builder->create<mlir::LLVM::AllocaOp>(
            loc,
            ptrTy,
            arrayTy,
            one   // ✅ NOT "size", ALWAYS 1 for array object
        );

        // =====================================================
        // ZERO INDEX FOR GEP ROOT
        // =====================================================
        auto zero = builder->create<mlir::arith::ConstantIntOp>(loc, 0, 32);

        // =====================================================
        // STORE ELEMENTS
        // =====================================================
        for (size_t i = 0; i < elements.size(); i++) {

            auto idx = builder->create<mlir::arith::ConstantIntOp>(loc, i, 32);

            // =================================================
            // ✅ CORRECT GEP: (0, i)
            // =================================================
            auto gep = builder->create<mlir::LLVM::GEPOp>(
                loc,
                ptrTy,
                arrayTy,
                alloca,
                mlir::ValueRange{zero, idx}
            );

            builder->create<mlir::LLVM::StoreOp>(
                loc,
                elements[i],
                gep
            );
        }

        return alloca;
    }

    /// =============================================================
    /// Indexing
    /// =============================================================

    /// Array indexing:
    ///  - GEP to element pointer
    ///  - Load element
    mlir::Value ExpressionsHelper::index(
        mlir::Value arr_p,
        mlir::Value idx,
        mlir::Type elemTy
    ) {
        auto ptrTy = mlir::LLVM::LLVMPointerType::get(builder->getContext());

        auto gep = builder->create<mlir::LLVM::GEPOp>(
            loc,
            ptrTy,
            elemTy,
            arr_p,
            mlir::ValueRange{idx}
        );

        return builder->create<mlir::LLVM::LoadOp>(
            loc,
            elemTy,
            gep
        );
    }

    /// Tuple indexing:
    ///  - GEP using struct layout
    ///  - Load field value
    mlir::Value ExpressionsHelper::index(
        mlir::Value tuple_ptr,
        mlir::LLVM::LLVMStructType struct_ty,
        mlir::Type tar_ty,
        mlir::Value i
    ){
        auto ptrTy = mlir::LLVM::LLVMPointerType::get(builder->getContext());

        auto index = mlir::ValueRange{
            builder->create<mlir::arith::ConstantOp>(
                loc, builder->getI32IntegerAttr(0)),
            i
        };

        auto fieldPtr = builder->create<mlir::LLVM::GEPOp>(
            loc,
            ptrTy,
            struct_ty,
            tuple_ptr,
            index
        );

        return builder->create<mlir::LLVM::LoadOp>(
            loc,
            tar_ty,
            fieldPtr
        );
    }

    bool ExpressionsHelper::isConstArrayExpr(std::shared_ptr<ArrayExpr> arr) {

        for (auto &elem : arr->elements) {

            if (elem->exp_kind == ExprKind::Literal)
                continue;

            if (elem->exp_kind == ExprKind::Array) {
                if (!isConstArrayExpr(std::static_pointer_cast<ArrayExpr>(elem)))
                    return false;
                continue;
            }

            if (elem->exp_kind == ExprKind::Binary) {
                if (!isConstStringExpr(elem))
                    return false;
                continue;
            }

            return false;
        }

        return true;
    }

    bool ExpressionsHelper::isCompileTimeValue(mlir::Value v) {
        if (v.getDefiningOp<mlir::arith::ConstantOp>())
            return true;

        if (v.getDefiningOp<mlir::LLVM::AddressOfOp>())
            return true;

        if (v.getDefiningOp<mlir::arith::ConstantIntOp>())
            return true;

        if (v.getDefiningOp<mlir::arith::ConstantFloatOp>())
            return true;

        return false;
    }

    mlir::Value ExpressionsHelper::getOpDefault(BinaryOp op){
        /*
        
            PLUS  -> Additive
            MINUS -> Additive
            STAR  -> Multiplicative
            DIV   -> Multiplicative
            MOD   -> Modulo
            LT GT LTE GTE -> Comparison
            AND OR -> Logical
        
        */

        OpClass opClass;

        switch(op){
            case BinaryOp::ADD:
            case BinaryOp::SUB:{
                opClass = OpClass::Additive;
                break;
            }
            case BinaryOp::MUL:
            case BinaryOp::DIV: {
                opClass = OpClass::Multiplicative;
                break;
            }
            case BinaryOp::EQ:
            case BinaryOp::GE:
            case BinaryOp::GT:
            case BinaryOp::LE:
            case BinaryOp::LT:{
                opClass = OpClass::Comparison;
                break;
            }
            case BinaryOp::AND:
            case BinaryOp::OR:{
                opClass = OpClass::Logical;
                break;
            }
            case BinaryOp::MOD:{
                opClass = OpClass::Modulo;
                break;
            }
            default:   
                llvm_unreachable("Op cannot be padded with values");
        }

        switch(opClass) {

            case OpClass::Additive:
                return i32(0); 

            case OpClass::Multiplicative:
                return i32(1);

            case OpClass::Logical:
                return createBool("false");

            case OpClass::Comparison:
                llvm_unreachable("Comparisons cannot be padded");

            case OpClass::Modulo:
                llvm_unreachable("Modulo does not support padding");
        }


    }
    mlir::Value ExpressionsHelper::loadArrayElem(
        mlir::Value array,
        mlir::Type elemTy,
        mlir::Value index
    ) {

        auto ptrTy =
            mlir::LLVM::LLVMPointerType::get(
                builder->getContext()
            );

        auto elemPtr =
            builder->create<mlir::LLVM::GEPOp>(
                loc,
                ptrTy,
                elemTy,
                array,
                mlir::ValueRange{index}
            );

        return builder->create<mlir::LLVM::LoadOp>(
            loc,
            elemTy,
            elemPtr
        );
    }

    void ExpressionsHelper::storeArrayElem(
        mlir::Value array,
        mlir::Type elemTy,
        mlir::Value index,
        mlir::Value value
    ) {

        auto ptrTy =
            mlir::LLVM::LLVMPointerType::get(
                builder->getContext()
            );

        auto elemPtr =
            builder->create<mlir::LLVM::GEPOp>(
                loc,
                ptrTy,
                elemTy,
                array,
                mlir::ValueRange{index}
            );

        builder->create<mlir::LLVM::StoreOp>(
            loc,
            value,
            elemPtr
        );
    }

    void ExpressionsHelper::copyArray(
        mlir::Value dstPtr,
        mlir::Value srcPtr,
        std::shared_ptr<ArrayType> arrTy
    ) {

        // =====================================================
        // TYPES
        // =====================================================

        auto elemTy =
            this->types->getMLIRType(arrTy);

        elemTy =
            mlir::cast<mlir::LLVM::LLVMArrayType>(
                elemTy
            ).getElementType();

        // =====================================================
        // DIMENSIONS
        // =====================================================

        auto [rows, cols] =
            arrTy->dimensions();

        int total =
            rows * cols;

        auto zero =
            i32(0);

        auto one =
            i32(1);

        auto size =
            i32(total);

        // =====================================================
        // COPY LOOP
        // =====================================================

        auto loop =
            builder->create<mlir::scf::ForOp>(
                loc,
                zero,
                size,
                one
            );

        builder->setInsertionPointToStart(
            loop.getBody()
        );

        {
            auto i =
                loop.getInductionVar();

            // -------------------------------------------------
            // LOAD SOURCE ELEMENT
            // -------------------------------------------------

            auto srcElem =
                loadArrayElem(
                    srcPtr,
                    elemTy,
                    i
                );

            // -------------------------------------------------
            // STORE DEST ELEMENT
            // -------------------------------------------------

            storeArrayElem(
                dstPtr,
                elemTy,
                i,
                srcElem
            );
        }

        builder->setInsertionPointAfter(loop);
    }



    mlir::Value ExpressionsHelper::matrixRowPtr(
        mlir::Value arrPtr,
        mlir::Value index,
        std::shared_ptr<ArrayType> arrTy
    ) {

        auto ptrTy =
            ptrty();

        auto [rows, cols] =
            arrTy->dimensions();

        (void)rows;

        // =====================================================
        // ROW OFFSET
        // =====================================================

        auto rowStart =
            builder->create<mlir::arith::MulIOp>(
                loc,
                index,
                i32(cols)
            );

        // =====================================================
        // ELEMENT TYPE
        // =====================================================

        auto elemTy =
            this->types->getMLIRType(
                arrTy->elem
            );

        // =====================================================
        // ROW POINTER
        // =====================================================

        return builder->create<mlir::LLVM::GEPOp>(
            loc,
            ptrTy,
            elemTy,
            arrPtr,
            mlir::ValueRange{ rowStart }
        );
    }

    mlir::Value ExpressionsHelper::arrayElementPtr(
        mlir::Value arrPtr,
        mlir::Value index,
        std::shared_ptr<ArrayType> arrTy
    ) {

        auto ptrTy =
            ptrty();

        auto elemTy =
            this->types->getMLIRType(
                arrTy->elem
            );

        return builder->create<mlir::LLVM::GEPOp>(
            loc,
            ptrTy,
            elemTy,
            arrPtr,
            mlir::ValueRange{ index }
        );
    }
}