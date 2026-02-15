set(
  rex_src_files
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_ast.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_ast_build.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_resolver.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/rex_scope.cpp"
)

add_executable(rex ${rex_src_files})
target_include_directories(rex PUBLIC ${ANTLR_GEN_DIR} "${CMAKE_SOURCE_DIR}/include")
add_dependencies(rex parser)
target_link_libraries(rex parser antlr4-runtime)
symlink_to_bin("rex")
