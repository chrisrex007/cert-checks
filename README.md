# cert-checks

- Established and organized a custom Clang-Tidy module with modular check registration, targeting rules EXP00-C, ARR02-C & PRE06-C
- Authored custom static analysis checks for the LLVM-based Clang-Tidy framework to enforce CERT C secure coding standards & practices
- Integrated the checks into a modular Clang-Tidy plugin using C++ with proper registration and a CMake-based build configuration
- Designed unit tests for each static check using Clang’s testing Framework, incorporating edge-case test cases to verify correctness
- Crafted comprehensive, reproducible build and test automation scripts using Bash, structured for easy integration into future CI pipelines
