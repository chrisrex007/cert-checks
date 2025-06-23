#include "CertChecksModule.h"
#include "checks/Arr02CCheck.h"
#include "checks/Pre06CCheck.h"
#include "checks/Exp00CCheck.h"

// Add this to prevent command line option conflicts
#include "llvm/Support/CommandLine.h"

// Disable problematic options before they get registered
namespace {
  struct DisableProblematicOptions {
    DisableProblematicOptions() {
      // This prevents the duplicate registration error
      llvm::cl::getGeneralCategory();
    }
  };
  static DisableProblematicOptions Disabler;
}

namespace clang {
  namespace tidy {
    namespace cert {
      void CertModule::addCheckFactories(ClangTidyCheckFactories& CheckFactories) {
        CheckFactories.registerCheck<Arr02CCheck>("cert-arr02-c");
        CheckFactories.registerCheck<Pre06CCheck>("cert-pre06-c");
        CheckFactories.registerCheck<Exp00CCheck>("cert-exp00-c");
      }

    } // namespace cert
  } // namespace tidy
} // namespace clang

// Register the module
static clang::tidy::ClangTidyModuleRegistry::Add<clang::tidy::cert::CertModule>
X("cert-module", "Adds CERT C coding standard checks.");

// This is required for dynamically loaded modules
volatile int CertModuleAnchorSource = 0;