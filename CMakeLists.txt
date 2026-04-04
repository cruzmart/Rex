cmake_minimum_required(VERSION 3.28.3)
project(Rex)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(ExternalProject)
find_package(Git REQUIRED)
find_package(Java COMPONENTS Runtime REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Wall -Wno-overloaded-virtual)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# 🔥 ADD THIS (MLIR/LLVM PATHS)
set(MLIR_DIR "/home/unknown/llvm-project/build/lib/cmake/mlir")
set(LLVM_DIR "/home/unknown/llvm-project/build/lib/cmake/llvm")

# 🔥 INCLUDE MLIR CONFIG
include("${CMAKE_SOURCE_DIR}/cmake/get_mlir.cmake")

include("${CMAKE_SOURCE_DIR}/cmake/symlink_to_bin.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/get_antlr.cmake")

set(GRAMMAR_NAME "Rex")
set(ANTLR_NAMESPACE "rex")

include("${CMAKE_SOURCE_DIR}/cmake/rex_generate.cmake")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/gen/rex
)

add_subdirectory("${CMAKE_SOURCE_DIR}/src")