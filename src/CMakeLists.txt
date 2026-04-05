set(
  rex_src_files
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_ast.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_exps.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_funcs.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_stmts.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_ast_nodes.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_ast_build.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_scope.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/passes/rex_alias_pass.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/passes/rex_exp_pass.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/passes/rex_return_pass.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/passes/rex_break_pass.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/passes/rex_assignment_pass.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_binary_op.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_errors.cpp"

    "${CMAKE_CURRENT_SOURCE_DIR}/backend/rex_backend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/backend/rex_print.cpp"

)

add_executable(rex ${rex_src_files})

# 🔥 INCLUDE DIRS (UPDATED)
target_include_directories(rex PUBLIC
    ${ANTLR_GEN_DIR}
    "${CMAKE_SOURCE_DIR}/include"
    ${MLIR_INCLUDE_DIRS}
    ${LLVM_INCLUDE_DIRS}
)

add_dependencies(rex parser)

# LLVM components
set(LLVM_LINK_COMPONENTS Core Support)
llvm_map_components_to_libnames(llvm_libs core)

# MLIR dialect libraries (AUTO)
get_property(dialect_libs GLOBAL PROPERTY MLIR_DIALECT_LIBS)

# Link everything cleanly
target_link_libraries(rex PRIVATE
    parser
    antlr4-runtime
    ${llvm_libs}
    ${dialect_libs}
)

symlink_to_bin("rex")