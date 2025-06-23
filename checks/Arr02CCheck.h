#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CERT_ARR02C_CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CERT_ARR02C_CHECK_H

#include "clang-tidy/ClangTidyCheck.h"

namespace clang {
  namespace tidy {
    namespace cert {

      /// Checks for ARR02-C: Explicitly specify array bounds, even if implicitly 
      /// defined by an initializer.
      /// 
      /// This checker identifies arrays declared without explicit bounds when they 
      /// could benefit from them for code clarity and maintainability.
      class Arr02CCheck : public ClangTidyCheck {
      public:
        Arr02CCheck(StringRef Name, ClangTidyContext* Context)
          : ClangTidyCheck(Name, Context) {
        }

        void registerMatchers(ast_matchers::MatchFinder* Finder) override;
        void check(const ast_matchers::MatchFinder::MatchResult& Result) override;
      };

    } // namespace cert
  } // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CERT_ARR02C_CHECK_H