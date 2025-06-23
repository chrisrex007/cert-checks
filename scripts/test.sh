#!/bin/bash
# Always load the custom CertChecks module and enable cert-* checks
MODULE_PATH="$(dirname "$0")/../build/CertChecks.so"
MODULE_PATH="$(readlink -f "$MODULE_PATH")"

export PATH=$HOME/clang-llvm/build/bin:$PATH
export LD_LIBRARY_PATH=$HOME/clang-llvm/build/lib:$LD_LIBRARY_PATH

exec clang-tidy -load="$MODULE_PATH" -checks='cert-*' -header-filter=.* "$@"
