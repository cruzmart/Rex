
#include "backend/rex_backend_visit.h"
#include "rex_ast.h"
#include "rex_ast_nodes.h"
#include "rex_exps.h"
#include "rex_stmts.h"
#include "rex_types.h"
#include <memory>


#include <mlir/Dialect/LLVMIR/LLVMDialect.h>
#include <mlir/IR/ValueRange.h>

namespace rex {
/// =============================================================
/// IRGen
/// =============================================================

IRGen::IRGen(
    std::shared_ptr<mlir::OpBuilder> b,
    mlir::ModuleOp &m,
    mlir::Location l,
    std::shared_ptr<TypesHelper> t
)
    : builder(std::move(b)),
      module(m),
      loc(l),
      types(std::move(t)) {}


/// =============================================================
/// ROOT VISIT
/// =============================================================

/// Entry point: visits entire file AST.
/// Initializes global scope and processes top-level statements.
void IRGen::visit(std::shared_ptr<FileAst> file) {

    currentScope = std::make_shared<Scope>();

    for (auto &item : file->items) {

        auto *block = builder->getInsertionBlock();

        if (!block || isTerminated(block))
            return;

        if (item->ast_kind == AstNodeKind::Stmt) {
            visitStmt(cast<Stmt>(item));
        }
    }
}


/// =============================================================
/// EXPRESSIONS
/// =============================================================

mlir::Value IRGen::visitExp(
    std::shared_ptr<Expr> expr
) {
    switch (expr->exp_kind) {

        case ExprKind::Literal:
            return visitLiteral(
                cast<LiteralExpr>(expr)
            );

        case ExprKind::Binary:
            return visitBinary(
                cast<BinaryExpr>(expr)
            );

        case ExprKind::Array:
            return visitArray(
                cast<ArrayExpr>(expr)
            );

        case ExprKind::Index:
            return visitIndex(
                cast<IndexExpr>(expr)
            );

        case ExprKind::TupleIndex:
            return visitIndexTuple(
                cast<IndexTupleExpr>(expr)
            );

        case ExprKind::Tuple:
            return visitTuple(
                cast<TupleExpr>(expr)
            );

        case ExprKind::Id:
            return visitId(
                cast<IdExpr>(expr)
            );

        default:
            return {};
    }
}

/// Literal → delegate to ExpressionsHelper
mlir::Value
IRGen::visitLiteral(
    std::shared_ptr<LiteralExpr> literal
) {
    return exps->createPrimitiveLiteral(literal);
}


/// Binary expression handling.
/// Includes:
///  - constant string folding
///  - primitive arithmetic
///  - automatic index loads
///  - (future) array operations
mlir::Value IRGen::visitBinary(
    std::shared_ptr<BinaryExpr> expr
) {

    auto op = expr->operation;


    // =========================================================
    // Constant string folding
    // =========================================================

    if (op == BinaryOp::ADD &&
        exps->isConstStringExpr(expr)) {

        return exps->createString(
            exps->foldConstString(expr)
        );
    }


    auto lhs = loadIfPointer(
        visitExp(expr->lhs),
        expr->lhs->type
    );

    auto rhs = loadIfPointer(
        visitExp(expr->rhs),
        expr->rhs->type
    );


    auto lhs_t = expr->lhs->type;
    auto rhs_t = expr->rhs->type;
    

    // For this step, lhs will give you what you need, if you are doing 2 + 2 = 4, if it is a index + index in which both are scalars is good, 
    // when we get to vector + scalar and etc. We have to do extra things.


    // =====================================================
    // Vector Exp Operation
    // =====================================================
    // GOt to fix this
    if(expr->type->kind == TypeKind::Array){

        auto bi_t = cast<ArrayType>(expr->type);

        bool lhsIsArray = lhs_t->kind == TypeKind::Array;
        bool rhsIsArray = rhs_t->kind == TypeKind::Array;

        bool lhsIsVector = false;
        bool rhsIsVector = false;

        if (lhsIsArray)
            lhsIsVector = cast<ArrayType>(lhs_t)->isVector();

        if (rhsIsArray)
            rhsIsVector = cast<ArrayType>(rhs_t)->isVector();

        // -----------------------------------------------------
        // Vector + Scalar
        // -----------------------------------------------------
        // (lhs = vector and rhs = scalar)
        if (lhsIsVector && !rhsIsArray) 
            return exps->createVectorScalarOp(lhs, lhs_t, rhs, rhs_t, op, true, cast<ArrayType>(expr->type));
        // (lhs = scalar and rhs = vector)
        if (!lhsIsArray && rhsIsVector) 
            return exps->createVectorScalarOp(lhs, lhs_t, rhs, rhs_t, op, false, cast<ArrayType>(expr->type));

        // -----------------------------------------------------
        // Vector + Vector
        // -----------------------------------------------------
        if (lhsIsVector && rhsIsVector) {

            auto lhsArr = cast<ArrayType>(lhs_t);
            auto rhsArr = cast<ArrayType>(rhs_t);


            return exps->createVectorVectorOp(lhs, lhsArr, rhs, rhsArr, op,  cast<ArrayType>(expr->type));
        }

        llvm_unreachable("Invalid array dispatch state");
    }
    // Todo: Find out which lhs or rhs is the matrix, and whatever values is on the the opposite, and do the operation accorgly. List
    
    // Vector + Scalar
    // Vector + Vector (Must be same dimensions)
    // Vector + Index (Must load the value to be scalar, and got to check if the index is a vector or a scalar).

    /*
        Key Notes:
            - ALL VECTORS AND MATRICIES ARE FLATTEN (1D), I DID EXP PASSES BEFORE, SO WE DON'T NEED TO DUPLICATE CODE, WE ALREADY DEAL WITH INCORRECT TYPES OPERATING ON EACH OTHER
    
    */


    
    // =====================================================
    // Regular Operation
    // =====================================================


    switch (expr->type->kind) {

        case TypeKind::Primitive: {

            auto prim =
                cast<PrimType>(expr->type);

            if (prim->prim ==
                PrimType::Prims::String) {

                llvm::report_fatal_error(
                    "Runtime string concat "
                    "not implemented"
                );
            }

            return exps->createBinaryExp(
                lhs,
                rhs,
                prim->prim,
                op
            );
        }

        case TypeKind::Array:
            return {};

        default:
            return {};
    }
}


/// =============================================================
/// ARRAY + TUPLE EXPRESSIONS
/// =============================================================

/// Handles array construction.
/// Distinguishes:
///  - compile-time constant arrays → global
///  - runtime arrays → stack allocation
mlir::Value IRGen::visitArray(
    std::shared_ptr<ArrayExpr> arr
) {

    auto arrTy = cast<ArrayType>(arr->type);

    std::vector<mlir::Value> values;

    auto flatten =
        [&](std::shared_ptr<ArrayExpr> a) {

        for (auto &elem : a->elements) {

            if (arrTy->isMatrix()) {

                auto row =
                    cast<ArrayExpr>(elem);

                for (auto &v : row->elements) {
                    values.push_back(
                        visitExp(v)
                    );
                }
            }
            else {
                values.push_back(
                    visitExp(elem)
                );
            }
        }
    };

    flatten(arr);

    arrTy->arrayKind =
        ArrayStorageKind::RuntimeAlloc;

    return exps->createRuntimeArray(
        values,
        arrTy->isMatrix()
            ? arrTy->matrixType()
            : arrTy->vectorType()
    );
}

/// Tuple creation → delegates to ExpressionsHelper
mlir::Value IRGen::visitTuple(
    std::shared_ptr<TupleExpr> tup
) {

    auto tupleTy =
        cast<TupleType>(tup->type);

    std::vector<mlir::Type> mlirTypes;
    std::vector<mlir::Value> values;

    for (auto &type : tupleTy->elements) {
        mlirTypes.push_back(
            types->getMLIRType(type)
        );
    }

    for (auto &elem : tup->elements) {
        values.push_back(
            visitExp(elem)
        );
    }

    auto tupTy = types->getMLIRType(tupleTy);

    return exps->createTuple(
          mlir::cast<mlir::LLVM::LLVMStructType>(tupTy),
        values
    );
}

/// Array indexing:
///  - evaluate base pointer
///  - compute index
///  - either:
///      * load scalar element
///      * return row pointer for matrix row access
mlir::Value IRGen::visitIndex(std::shared_ptr<IndexExpr> i) {

    mlir::Value arrPtr = visitExp(i->base);
    mlir::Value index  = visitExp(i->index);
    auto arrTy = std::static_pointer_cast<ArrayType>(i->base->type);
    
    // =====================================================
    // MATRIX CASE
    // =====================================================
    //
    // matrix[row]
    //
    // We DO NOT load.
    // We return pointer to start of row.
    //
    // Since matrix is flattened:
    //
    // linear_index = row * cols
    //
    // Then GEP to that offset.
    //
    // Example:
    //
    // [[1,2,3],
    //  [4,5,6]]
    //
    // stored as:
    //
    // [1,2,3,4,5,6]
    //
    // matrix[1]
    // -> pointer to element 3
    // -> [4,5,6]
    //
    // =====================================================

    if (arrTy->isMatrix())
        return exps->matrixRowPtr(arrPtr, index, arrTy);

    // =====================================================
    // NORMAL ARRAY CASE 
    // =====================================================

    return exps->arrayElementPtr(arrPtr, index, arrTy);

}

/// Tuple indexing:
/// Uses struct layout + GEP
mlir::Value IRGen::visitIndexTuple(
    std::shared_ptr<IndexTupleExpr> idx
) {

    auto tupPtr = visitExp(idx->base);
    auto field  = visitExp(idx->field);

    if (idx->base->exp_kind !=
        ExprKind::Tuple) {
        return {};
    }

    auto tupleTy =
        cast<TupleType>(idx->base->type);

    std::vector<mlir::Type> fields;

    for (auto &type : tupleTy->elements) {
        fields.push_back(
            types->getMLIRType(type)
        );
    }

    auto structTy =
        mlir::LLVM::LLVMStructType::getLiteral(
            builder->getContext(),
            fields
        );

    return exps->index(
        tupPtr,
        structTy,
        fields[idx->field_index],
        field
    );
}



/// Variable access:
///  - arrays → return pointer
///  - scalars → load value
mlir::Value IRGen::visitId(
    std::shared_ptr<IdExpr> id
) {

    auto sym =
        currentScope->resolve(id->name);

    if (sym->kind != SymbolType::Variable)
        llvm_unreachable("Not variable");

    auto var =
        cast<VariableSymbol>(sym);

    if (!var->ptr)
        llvm_unreachable("Missing storage");


    
    if (sym->type->kind ==
        TypeKind::Array) {
        return var->ptr;
    }

      
    if (sym->type->kind ==
        TypeKind::Tuple) {
         return builder->create<mlir::LLVM::LoadOp>(
        loc,
        types->ptrty(),
        var->ptr);
    }


    return builder->create<mlir::LLVM::LoadOp>(
        loc,
        types->getMLIRType(sym->type),
        var->ptr
   );
}


/// =============================================================
/// STATEMENTS
/// =============================================================

void IRGen::visitStmt(
    std::shared_ptr<Stmt> stmt
) {

    switch (stmt->stmt_kind) {

        case StmtKind::Print:
            return visitPrint(
                cast<PrintStmt>(stmt)
            );

        case StmtKind::If:
            return visitIf(
                cast<IfStmt>(stmt)
            );

        case StmtKind::While:
            return visitWhile(
                cast<WhileStmt>(stmt)
            );

        case StmtKind::Loop:
            return visitLoop(
                cast<LoopStmt>(stmt)
            );

        case StmtKind::For:
            return visitFor(
                cast<ForStmt>(stmt)
            );

        case StmtKind::Break:
            return visitBreak(
                cast<BreakStmt>(stmt)
            );

        case StmtKind::LetDecl:
            return visitDelc(
                cast<LetStmt>(stmt)
            );

        case StmtKind::Assign:
            return visitAssign(
                cast<AssignStmt>(stmt)
            );

        default:
            return;
    }
}


/// =============================================================
/// VARIABLE STORAGE
/// =============================================================
// Have to visit this back lowkey.
/// Allocates stack storage for a variable (LLVM alloca)
mlir::Value IRGen::allocateStorage(
    std::shared_ptr<Type> type,
    mlir::Type ptrTy,
    mlir::Value size
) {
    auto elemTy = types->getMLIRType(type);
    return builder->create<mlir::LLVM::AllocaOp>(
        loc,
        ptrTy,
        elemTy,
        size
    );
}


/// Initializes storage:
///  - arrays → deep copy
///  - scalars → direct store
void IRGen::initializeStorage(
    mlir::Value dst,
    mlir::Value src,
    std::shared_ptr<Type> type
) {

    if (type->kind != TypeKind::Array) {

        builder->create<mlir::LLVM::StoreOp>(
            loc,
            src,
            dst
        );

        return;
    }

    auto arr = cast<ArrayType>(type);

    exps->copyArray(
        dst,
        src,
        arr
    );
}

mlir::Value IRGen::materializeValueForStorage(
    std::shared_ptr<Expr> expr,
    mlir::Value value
) {
    // Index expressions can produce either:
    //   - a scalar pointer that must be loaded
    //   - a row pointer from matrix indexing that should stay a pointer
    //
    // Matrix row indexing:
    //     matrix[i]
    // returns a row pointer.
    //
    // Nested indexing:
    //     matrix[i][j]
    // returns a scalar pointer and must be loaded.

    if (expr->exp_kind != ExprKind::Index)
        return value;

    auto indexExpr =
        cast<IndexExpr>(expr);

    bool isNestedIndex =
        indexExpr->base->exp_kind ==
        ExprKind::Index;

    if (!isNestedIndex)
        return value;

    return builder->create<mlir::LLVM::LoadOp>(
        loc,
        types->getMLIRType(indexExpr->type),
        value
    );
}

// =====================================================
// VARIABLE DECLERATION 
// =====================================================
// This needs to be fixed and updated.

void IRGen::visitDelc(
    std::shared_ptr<LetStmt> stmt
) {
    auto ptrTy = types->ptrty();

    auto one = i32(1);

    // =========================================================
    // Single binding
    // =========================================================

    if (stmt->id_pattern->pat_type ==
        PatternType::Single) {

        auto id =
            cast<PatternId>(
                stmt->id_pattern
            );

        auto value =
            visitExp(stmt->exp);

        value = materializeValueForStorage(
            stmt->exp,
            value
        );

        auto storage =
            allocateStorage(
                stmt->type,
                ptrTy,
                one
            );

        initializeStorage(
            storage,
            value,
            stmt->type
        );

        currentScope->define(
            std::make_shared<VariableSymbol>(
                id->id,
                stmt->type,
                storage
            )
        );

        return;
    }

    // =========================================================
    // Tuple destructuring
    // =========================================================

    auto ids =
        cast<PatternIds>(
            stmt->id_pattern
        )->ids;

    auto tupleExpr =
        cast<TupleExpr>(stmt->exp);

    auto tupleTy =
        cast<TupleType>(tupleExpr->type);

    for (size_t i = 0; i < ids.size(); ++i) {

        auto value =
            visitExp(tupleExpr->elements[i]);

        value = materializeValueForStorage(
            tupleExpr->elements[i],
            value
        );

        auto storage =
            allocateStorage(
                tupleTy->elements[i],
                ptrTy,
                one
            );

        initializeStorage(
            storage,
            value,
            tupleTy->elements[i]
        );

        currentScope->define(
            std::make_shared<VariableSymbol>(
                ids[i],
                tupleTy->elements[i],
                storage
            )
        );
    }
}

// =====================================================
// VARIABLE REASSIGNMENT
// =====================================================

void IRGen::visitAssign(
    std::shared_ptr<AssignStmt> stmt
) {

    auto value =
        visitExp(stmt->value);

    // =========================================================
    // Variable assignment
    // =========================================================

    if (stmt->target->exp_kind ==
        ExprKind::Id) {

        auto id =
            cast<IdExpr>(stmt->target);

        auto sym =
            currentScope->resolve(
                id->name
            );

        if (!sym ||
            sym->kind !=
            SymbolType::Variable) {

            llvm_unreachable(
                "Invalid assignment target"
            );
        }

        auto var =
            cast<VariableSymbol>(sym);

        if (sym->type->kind ==
            TypeKind::Array) {

            exps->copyArray(
                var->ptr,
                value,
                cast<ArrayType>(sym->type)
            );

            return;
        }


        builder->create<mlir::LLVM::StoreOp>(
            loc,
            value,
            var->ptr
        );

        return;
    }

    // =========================================================
    // Indexed assignment
    // =========================================================

    if (stmt->target->exp_kind ==
        ExprKind::Index) {

        auto idx =
            cast<IndexExpr>(
                stmt->target
            );

        auto ptr =
            visitIndex(idx);

        if (idx->type->kind ==
            TypeKind::Array) {

            exps->copyArray(
                ptr,
                value,
                cast<ArrayType>(idx->type)
            );

            return;
        }

        if (stmt->value->exp_kind ==
            ExprKind::Index) {

            auto loaded =
                builder->create<mlir::LLVM::LoadOp>(
                    loc,
                    types->getMLIRType(stmt->value->type),
                    value
                );
            llvm::errs() << types->getMLIRType(stmt->value->type) << "\n";
            builder->create<mlir::LLVM::StoreOp>(
                loc,
                loaded,
                ptr
            );

            return;
        }


        builder->create<mlir::LLVM::StoreOp>(
            loc,
            value,
            ptr
        );

        return;
    }

    llvm_unreachable(
        "Unsupported assignment target"
    );
}

// =====================================================
// PRINT
// =====================================================

void IRGen::visitPrint(
    std::shared_ptr<PrintStmt> stmt
) {

    // =====================================================
    // SPECIAL CASE:
    // matrix[row]
    // =====================================================

    if (stmt->argument->exp_kind ==
        ExprKind::Index) {

        auto idx =
            cast<IndexExpr>(
                stmt->argument
            );

        prints->printIndexed(
            visitExp(stmt->argument),
            idx
        );

        return;
    }

    // =====================================================
    // EVALUATE
    // =====================================================

    auto value =
        visitExp(stmt->argument);

    auto type =
        stmt->argument->type;

    // =====================================================
    // PRINT BY TYPE
    // =====================================================

    switch (type->kind) {

        case TypeKind::Primitive:

            prints->printInline(value);
            break;

        case TypeKind::Array:

            prints->printArray(
                value,
                cast<ArrayType>(type)
            );

            break;

        case TypeKind::Tuple: {

            auto tupleTy =
                cast<TupleType>(type);

            auto structTy = mlir::cast<mlir::LLVM::LLVMStructType>(types->getMLIRType(tupleTy));

            prints->printTuple(
                value,
                structTy,
                tupleTy->elements
            );

            break;
        }

        default:
            llvm::report_fatal_error(
                "Unsupported type in print"
            );
    }

    // =====================================================
    // NEWLINE
    // =====================================================

    auto nl =
        builder->create<
            mlir::arith::ConstantIntOp
        >(
            loc,
            '\n',
            8
        );

    builder->create<mlir::LLVM::CallOp>(
        loc,
        prints->printf_func,
        mlir::ValueRange{
            prints->getFmtAddress(
                prints->fmt_char
            ),
            nl
        }
    );
}

}