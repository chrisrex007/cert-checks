#ifndef CERT_CHECKS_MODULE_H
#define CERT_CHECKS_MODULE_H

#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"

namespace clang {
  namespace tidy {
    namespace cert {

      class CertModule : public ClangTidyModule {
      public:
        void addCheckFactories(ClangTidyCheckFactories& CheckFactories) override;
      };

    } // namespace cert
  } // namespace tidy  
} // namespace clang

#endif // CERT_CHECKS_MODULE_H