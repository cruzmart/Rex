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
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_binary_op.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_errors.cpp"
    
)

add_executable(rex ${rex_src_files})
target_include_directories(rex PUBLIC ${ANTLR_GEN_DIR} "${CMAKE_SOURCE_DIR}/include")
add_dependencies(rex parser)
target_link_libraries(rex parser antlr4-runtime)
symlink_to_bin("rex")
