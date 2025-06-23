#include "Exp00CCheck.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/MacroArgs.h"
#include <cctype>

namespace clang {
  namespace tidy {
    namespace cert {

      class Exp00CCheck::Exp00CPPCallbacks : public PPCallbacks {
      public:
        Exp00CPPCallbacks(Exp00CCheck& Check, const SourceManager& SM)
          : Check(Check), SM(SM) {
        }

        void MacroDefined(const Token& MacroNameTok,
          const MacroDirective* MD) override {
          const MacroInfo* MI = MD->getMacroInfo();
          if (!MI || !MI->isFunctionLike())
            return;

          StringRef MacroName = MacroNameTok.getIdentifierInfo()->getName();
          SourceLocation StartLoc = MI->getDefinitionLoc();

          if (StartLoc.isInvalid())
            return;

          // Analyze the macro for issues
          bool ReplacementNeedsParens = analyzeReplacementList(MI);
          bool ParametersNeedParens = analyzeParameterUsage(MI);

          if (ReplacementNeedsParens) {
            Check.diag(StartLoc,
              "macro replacement list should be enclosed in parentheses (EXP00-C)")
              << MacroName;
            // Optional: Suggest fix-it to wrap replacement list in parentheses
          }
          if (ParametersNeedParens) {
            Check.diag(StartLoc,
              "macro parameters should be enclosed in parentheses (EXP00-C)")
              << MacroName;
            // Optional: Suggest fix-it to wrap parameter in parentheses
          }
        }

      private:
        bool analyzeReplacementList(const MacroInfo* MI) {
          auto Tokens = MI->tokens();

          if (Tokens.empty())
            return false;

          // Check if replacement contains operators that need outer parentheses
          bool HasOperators = false;
          for (const auto& Token : Tokens) {
            if (Token.is(tok::plus) || Token.is(tok::minus) ||
              Token.is(tok::star) || Token.is(tok::slash) ||
              Token.is(tok::percent) || Token.is(tok::amp) ||
              Token.is(tok::pipe) || Token.is(tok::caret)) {
              HasOperators = true;
              break;
            }
          }

          // If we have operators, check if whole expression is parenthesized
          if (HasOperators) {
            // Check if the entire replacement is wrapped in a single pair of parentheses
            if (Tokens.front().is(tok::l_paren) && Tokens.back().is(tok::r_paren)) {
              int parenDepth = 0;
              for (size_t i = 0; i < Tokens.size(); ++i) {
                if (Tokens[i].is(tok::l_paren)) parenDepth++;
                if (Tokens[i].is(tok::r_paren)) parenDepth--;
                // If we close the outermost paren before the last token, not fully wrapped
                if (parenDepth == 0 && i != Tokens.size() - 1)
                  return true; // Not fully parenthesized
              }
              if (parenDepth == 0)
                return false; // Fully parenthesized
            }
            return true; // Not parenthesized at all
          }

          return false;
        }

        bool analyzeParameterUsage(const MacroInfo* MI) {
          auto Tokens = MI->tokens();

          if (Tokens.empty() || MI->param_empty())
            return false;

          // Build set of parameter names for quick lookup
          llvm::SmallSet<StringRef, 8> ParamNames;
          for (const IdentifierInfo* Param : MI->params()) {
            if (Param)
              ParamNames.insert(Param->getName());
          }

          // Check each parameter usage
          for (auto It = Tokens.begin(); It != Tokens.end(); ++It) {
            if (!It->is(tok::identifier))
              continue;

            StringRef TokenName = It->getIdentifierInfo()->getName();
            if (!ParamNames.count(TokenName))
              continue;

            // Found parameter usage - check if it needs parentheses
            bool NeedsParens = false;

            // Check previous token
            if (It != Tokens.begin()) {
              auto PrevIt = std::prev(It);
              if (PrevIt->is(tok::star) || PrevIt->is(tok::slash) ||
                PrevIt->is(tok::percent) || PrevIt->is(tok::plus) ||
                PrevIt->is(tok::minus)) {
                NeedsParens = true;
              }
            }

            // Check next token
            auto NextIt = std::next(It);
            if (NextIt != Tokens.end()) {
              if (NextIt->is(tok::star) || NextIt->is(tok::slash) ||
                NextIt->is(tok::percent) || NextIt->is(tok::plus) ||
                NextIt->is(tok::minus)) {
                NeedsParens = true;
              }
            }

            // Check if parameter is already parenthesized
            if (NeedsParens) {
              bool IsParenthesized = false;

              if (It != Tokens.begin() && NextIt != Tokens.end()) {
                auto PrevIt = std::prev(It);
                if (PrevIt->is(tok::l_paren) && NextIt->is(tok::r_paren)) {
                  IsParenthesized = true;
                }
              }

              if (!IsParenthesized)
                return true;
            }
          }

          return false;
        }

        Exp00CCheck& Check;
        const SourceManager& SM;
      };

      void Exp00CCheck::registerPPCallbacks(const SourceManager& SM,
        Preprocessor* PP,
        Preprocessor* ModuleExpanderPP) {
        PP->addPPCallbacks(std::make_unique<Exp00CPPCallbacks>(*this, SM));
      }

    } // namespace cert
  } // namespace tidy
} // namespace clang