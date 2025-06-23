#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CERT_EXP00C_CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CERT_EXP00C_CHECK_H

#include "clang-tidy/ClangTidyCheck.h"

namespace clang {
  namespace tidy {
    namespace cert {

      /// Checks for EXP00-C: Use parentheses for macro parameters and the entire 
      /// replacement list.
      /// 
      /// This checker identifies macros that don't properly parenthesize parameters 
      /// and replacement lists, which can lead to unexpected behavior due to 
      /// operator precedence issues.
      class Exp00CCheck : public ClangTidyCheck {
      public:
        Exp00CCheck(StringRef Name, ClangTidyContext* Context)
          : ClangTidyCheck(Name, Context) {
        }

        void registerPPCallbacks(const SourceManager& SM, Preprocessor* PP,
          Preprocessor* ModuleExpanderPP) override;

      private:
        class Exp00CPPCallbacks;
      };

    } // namespace cert
  } // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CERT_EXP00C_CHECK_H