# Find MLIR + LLVM
find_package(MLIR REQUIRED CONFIG)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using MLIRConfig.cmake in: ${MLIR_DIR}")

# Include dirs
include_directories(SYSTEM "${MLIR_INCLUDE_DIRS}")
include_directories(SYSTEM "${LLVM_INCLUDE_DIRS}")

# 🔥 THIS FIXES YOUR LINKER ISSUE
link_directories(${LLVM_LIBRARY_DIRS})

# Definitions
add_definitions(${MLIR_DEFINITIONS})

