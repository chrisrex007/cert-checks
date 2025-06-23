#include "Arr02CCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/Diagnostic.h"

using namespace clang::ast_matchers;

namespace clang {
  namespace tidy {
    namespace cert {

      void Arr02CCheck::registerMatchers(MatchFinder* Finder) {
        // Match all array variable declarations with an initializer
        Finder->addMatcher(
          varDecl(
            hasType(arrayType()),
            hasInitializer(anything())
          ).bind("array_decl"),
          this);
      }

      void Arr02CCheck::check(const MatchFinder::MatchResult& Result) {
        const auto* VD = Result.Nodes.getNodeAs<VarDecl>("array_decl");
        if (!VD)
          return;

        // Use TypeLoc to check if the array size was omitted in the source
        TypeSourceInfo* TSI = VD->getTypeSourceInfo();
        if (!TSI)
          return;

        TypeLoc TL = TSI->getTypeLoc();
        ArrayTypeLoc ATL = TL.getAs<ArrayTypeLoc>();
        if (!ATL)
          return;

        // If the brackets are empty, the size was omitted
        SourceLocation LBracketLoc = ATL.getLBracketLoc();
        SourceLocation RBracketLoc = ATL.getRBracketLoc();
        if (!LBracketLoc.isValid() || !RBracketLoc.isValid() ||
          LBracketLoc == RBracketLoc) {
          return;
        }
        // Check if there is any token between [ and ]
        if (LBracketLoc.getLocWithOffset(1) == RBracketLoc) {
          // Try to infer the number of elements
          size_t NumElements = 0;
          bool CanDetermineSize = false;

          const Expr* Init = VD->getInit();
          if (const auto* ILE = dyn_cast<InitListExpr>(Init)) {
            NumElements = ILE->getNumInits();
            CanDetermineSize = true;
          }
          else if (const auto* SL = dyn_cast<StringLiteral>(Init)) {
            NumElements = SL->getLength() + 1;
            CanDetermineSize = true;
          }

          if (!CanDetermineSize)
            return;

          diag(VD->getLocation(),
            "array bound should be explicitly specified (ARR02-C); "
            "inferred size is %0 elements")
            << static_cast<unsigned>(NumElements);

          std::string Replacement = std::to_string(NumElements);
          diag(LBracketLoc, "insert explicit array bound", DiagnosticIDs::Note)
            << FixItHint::CreateInsertion(
              LBracketLoc.getLocWithOffset(1), Replacement);
        }
      }

    } // namespace cert
  } // namespace tidy
} // namespace clang