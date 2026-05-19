set(REX_CORE
    main.cpp
    rex_ast.cpp
    rex_exps.cpp
    rex_funcs.cpp
    rex_stmts.cpp
    rex_ast_nodes.cpp
    rex_ast_build.cpp
    rex_scope.cpp
    rex_binary_op.cpp
    rex_errors.cpp
)

set(REX_PASSES
    passes/rex_alias_pass.cpp
    passes/rex_exp_pass.cpp
    passes/rex_return_pass.cpp
    passes/rex_break_pass.cpp
    passes/rex_assignment_pass.cpp
)

set(REX_BACKEND
    backend/rex_backend.cpp
    backend/rex_backend_prints.cpp
    backend/rex_backend_types.cpp
    backend/rex_backend_exps.cpp
    backend/rex_backend_exps_helpers.cpp
    backend/rex_backend_exps_math.cpp
    backend/rex_backend_exps_arrays.cpp
    backend/rex_backend_visit.cpp
    backend/rex_backend_visit_helpers.cpp
    backend/rex_backend_visit_flow_control.cpp
)

add_executable(rex)

target_sources(rex PRIVATE
    ${REX_CORE}
    ${REX_PASSES}
    ${REX_BACKEND}
)

target_include_directories(rex PUBLIC
    ${ANTLR_GEN_DIR}
    "${CMAKE_SOURCE_DIR}/include"
    ${MLIR_INCLUDE_DIRS}
    ${LLVM_INCLUDE_DIRS}
)

add_dependencies(rex parser)

set(LLVM_LINK_COMPONENTS Core Support)
llvm_map_components_to_libnames(llvm_libs core)

get_property(dialect_libs GLOBAL PROPERTY MLIR_DIALECT_LIBS)

target_link_libraries(rex PRIVATE
    parser
    antlr4-runtime
    ${llvm_libs}
    ${dialect_libs}
)

symlink_to_bin("rex")