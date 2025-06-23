#include "Pre06CCheck.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/ADT/DenseSet.h"
#include "clang/Basic/FileManager.h"

namespace clang {
  namespace tidy {
    namespace cert {

      class Pre06CCheck::Pre06CPPCallbacks : public PPCallbacks {
      public:
        Pre06CPPCallbacks(Pre06CCheck& Check, const SourceManager& SM)
          : Check(Check), SM(SM) {
        }

        void FileChanged(SourceLocation Loc, FileChangeReason Reason,
          SrcMgr::CharacteristicKind FileType,
          FileID PrevFID) override {
          if (Reason == EnterFile) {
            FileID FID = SM.getFileID(Loc);
            StringRef FileName = SM.getFilename(Loc);

            if (isHeaderFile(FileName)) {
              HeaderFiles.insert(FID);
            }
          }
        }

        void Ifndef(SourceLocation Loc, const Token& MacroNameTok,
          const MacroDefinition& MD) override {
          FileID FID = SM.getFileID(Loc);
          if (HeaderFiles.count(FID)) {
            GuardedHeaders.insert(FID);
          }
        }

        void If(SourceLocation Loc, SourceRange ConditionRange,
          ConditionValueKind ConditionValue) override {
          // Handle #if !defined(...) style guards
          FileID FID = SM.getFileID(Loc);
          if (HeaderFiles.count(FID)) {
            // This is a simplified check - in practice you'd want to parse
            // the condition to verify it's actually a header guard pattern
            std::string ConditionText = getSourceText(ConditionRange);
            if (ConditionText.find("!defined") != std::string::npos ||
              ConditionText.find("!DEFINED") != std::string::npos) {
              GuardedHeaders.insert(FID);
            }
          }
        }

        void PragmaDirective(SourceLocation Loc, PragmaIntroducerKind Introducer) override {
          // Handle #pragma once as a valid include guard
          FileID FID = SM.getFileID(Loc);
          if (HeaderFiles.count(FID)) {
            // Get the pragma text to check if it's "once"
            SourceLocation EndLoc = Lexer::getLocForEndOfToken(
              Loc, 0, SM, LangOptions());
            if (EndLoc.isValid()) {
              std::string PragmaText = getSourceText(SourceRange(Loc, EndLoc));
              if (PragmaText.find("once") != std::string::npos) {
                GuardedHeaders.insert(FID);
              }
            }
          }
        }

        void EndOfMainFile() override {
          for (FileID FID : HeaderFiles) {
            if (GuardedHeaders.find(FID) == GuardedHeaders.end()) {
              SourceLocation Loc = SM.getLocForStartOfFile(FID);
              StringRef FileName = SM.getFilename(Loc);

              if (FileName.empty() || SM.isInSystemHeader(Loc))
                continue;

              Check.diag(Loc,
                "header file '%0' should have include guard (PRE06-C)")
                << FileName;
              // Optional: Suggest fix-it to insert #ifndef/#define/#endif
            }
          }
        }

      private:
        bool isHeaderFile(StringRef FileName) const {
          return FileName.ends_with(".h") || FileName.ends_with(".hpp") ||
            FileName.ends_with(".hxx") || FileName.ends_with(".hh") ||
            FileName.ends_with(".inc") || FileName.ends_with(".inl");
        }

        std::string getSourceText(SourceRange Range) const {
          if (Range.isInvalid())
            return "";

          const char* Begin = SM.getCharacterData(Range.getBegin());
          const char* End = SM.getCharacterData(Range.getEnd());

          if (!Begin || !End || Begin >= End)
            return "";

          return std::string(Begin, End - Begin);
        }

        Pre06CCheck& Check;
        const SourceManager& SM;
        llvm::DenseSet<FileID> HeaderFiles;
        llvm::DenseSet<FileID> GuardedHeaders;
      };

      void Pre06CCheck::registerPPCallbacks(const SourceManager& SM,
        Preprocessor* PP,
        Preprocessor* ModuleExpanderPP) {
        PP->addPPCallbacks(std::make_unique<Pre06CPPCallbacks>(*this, SM));
      }

    } // namespace cert
  } // namespace tidy
} // namespace clang