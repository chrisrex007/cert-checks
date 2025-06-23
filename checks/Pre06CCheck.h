#ifndef LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CERT_PRE06C_CHECK_H
#define LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CERT_PRE06C_CHECK_H

#include "clang-tidy/ClangTidyCheck.h"

namespace clang {
  namespace tidy {
    namespace cert {

      /// Checks for PRE06-C: Enclose header files in an include guard.
      /// 
      /// This checker identifies header files that lack proper include guards,
      /// which can lead to multiple inclusion problems and compilation errors.
      /// Recognizes both traditional #ifndef guards and #pragma once.
      class Pre06CCheck : public ClangTidyCheck {
      public:
        Pre06CCheck(StringRef Name, ClangTidyContext* Context)
          : ClangTidyCheck(Name, Context) {
        }

        void registerPPCallbacks(const SourceManager& SM, Preprocessor* PP,
          Preprocessor* ModuleExpanderPP) override;

      private:
        class Pre06CPPCallbacks;
      };

    } // namespace cert
  } // namespace tidy
} // namespace clang

#endif // LLVM_CLANG_TOOLS_EXTRA_CLANG_TIDY_CERT_PRE06C_CHECK_H