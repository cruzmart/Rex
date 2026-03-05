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
