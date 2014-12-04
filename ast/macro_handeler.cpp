
#include "macro_handeler.hpp"

PreprocessorCallback::PreprocessorCallback(Preprocessor &PP) : PP(PP) {}
PreprocessorCallback::~PreprocessorCallback() {}

void PreprocessorCallback::MacroExpands(const Token &MacroNameTok,
                                        const MacroInfo *MI,
                                        SourceRange Range) {}

void PreprocessorCallback::MacroExpands(const Token &MacroNameTok,
                                        const MacroDirective *MD,
                                        SourceRange Range,
                                        const MacroArgs *Args) {
  MacroExpands(MacroNameTok, MD->getMacroInfo(), Range);
}

void PreprocessorCallback::InclusionDirective(
    SourceLocation HashLoc, const Token &IncludeTok, llvm::StringRef FileName,
    bool IsAngled, CharSourceRange FilenameRange, const FileEntry *File,
    llvm::StringRef SearchPath, llvm::StringRef RelativePath,
    const Module *Imported) {

  // std::cout << "This is an include" << std::endl;
}
void PreprocessorCallback::If(SourceLocation Loc, SourceRange ConditionRange,
                              ConditionValueKind ConditionValue) {
  HandlePPCond(Loc, Loc);
}
void PreprocessorCallback::Ifndef(SourceLocation Loc, const Token &MacroNameTok,
                                  const MacroDirective *MD) {
  HandlePPCond(Loc, Loc);
}
void PreprocessorCallback::Ifdef(SourceLocation Loc, const Token &MacroNameTok,
                                 const MacroDirective *MD) {
  HandlePPCond(Loc, Loc);
}
void PreprocessorCallback::Elif(SourceLocation Loc, SourceRange ConditionRange,
                                ConditionValueKind ConditionValue,
                                SourceLocation IfLoc) {
  ElifMapping[Loc] = IfLoc;
  HandlePPCond(Loc, IfLoc);
}
void PreprocessorCallback::Else(SourceLocation Loc, SourceLocation IfLoc) {
  HandlePPCond(Loc, IfLoc);
}
void PreprocessorCallback::Endif(SourceLocation Loc, SourceLocation IfLoc) {
  HandlePPCond(Loc, IfLoc);
}
bool
PreprocessorCallback::FileNotFound(llvm::StringRef FileName,
                                   llvm::SmallVectorImpl<char> &RecoveryPath) {
  if (!PP.GetSuppressIncludeNotFoundError()) {
    PP.SetSuppressIncludeNotFoundError(true);
  }
  return false;
}
