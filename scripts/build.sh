#!/usr/bin/env bash
# scripts/build.sh
set -euo pipefail

# Path to your LLVM/Clang build tree:
LLVM_BUILD_DIR="$HOME/clang-llvm/build"

# Discover the CMake package dirs
LLVM_CMAKE_DIR="$(find "$LLVM_BUILD_DIR" -type f -name LLVMConfig.cmake | head -n1 | xargs dirname)"
CLANG_CMAKE_DIR="$(find "$LLVM_BUILD_DIR" -type f -name ClangConfig.cmake | head -n1 | xargs dirname || true)"

if [[ -z "$LLVM_CMAKE_DIR" ]]; then
  echo "Error: LLVMConfig.cmake not found in $LLVM_BUILD_DIR"
  exit 1
fi

BUILD_DIR="$PWD/build"
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_DIR="$LLVM_CMAKE_DIR" \
  ${CLANG_CMAKE_DIR:+-DClang_DIR="$CLANG_CMAKE_DIR"} \
  -DCMAKE_C_COMPILER="$LLVM_BUILD_DIR/bin/clang" \
  -DCMAKE_CXX_COMPILER="$LLVM_BUILD_DIR/bin/clang++" \
  -DCMAKE_POSITION_INDEPENDENT_CODE=ON

cmake --build . --target CertChecks -j"$(nproc)"

# Extract & trim the suffix (e.g. ".so", ".dylib", or ".dll")
suffix=$(cmake --system-information \
         | grep CMAKE_SHARED_LIBRARY_SUFFIX \
         | cut -d'=' -f2 \
         | tr -d ' "')

module_path="$BUILD_DIR/CertChecks${suffix}"

echo ""
echo "Build succeeded. Module is at:"
echo "  $module_path"
echo ""
echo "To use with clang-tidy, run:"
echo "  clang-tidy -load=${module_path} -checks='cert-*' <your-file>.cpp"
