cmake_minimum_required(VERSION 3.13)
project(cert-checks LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find LLVM and Clang
find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Found Clang ${Clang_PACKAGE_VERSION}")

# Include directories and definitions
include_directories(${LLVM_INCLUDE_DIRS} ${CLANG_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS} ${CLANG_DEFINITIONS})

# Ensure we can find the cmake modules
list(APPEND CMAKE_MODULE_PATH "${LLVM_DIR}" "${Clang_DIR}")
include(AddLLVM)
include(AddClang)

add_library(CertChecks MODULE
  CertChecksModule.cpp
  checks/Arr02CCheck.cpp
  checks/Pre06CCheck.cpp
  checks/Exp00CCheck.cpp
)

set_target_properties(CertChecks PROPERTIES
  COMPILE_FLAGS "-fno-rtti"
  PREFIX ""
)