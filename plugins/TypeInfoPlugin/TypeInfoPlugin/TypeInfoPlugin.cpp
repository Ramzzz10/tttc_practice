#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Sema/Sema.h"
#include "llvm/Support/raw_ostream.h"
#include <string>
#include <set>
using namespace clang;

namespace {
class TypeInfoVisitor : public RecursiveASTVisitor<TypeInfoVisitor> {
private:
  ASTContext *Context;
  
public:
  explicit TypeInfoVisitor(ASTContext *Context) : Context(Context) {}


  std::string getAccessSpecifierString(AccessSpecifier AS) {
    switch (AS) {
      case AS_public:    return "public";
      case AS_protected: return "protected";
      case AS_private:   return "private";
      case AS_none:      return "none";
    }
    return "unknown";
  }


  std::string getMethodModifiersString(const CXXMethodDecl *Method) {
    std::string Modifiers;
    
    if (Method->isVirtual())
      Modifiers += "virtual|";
    
    if (Method->isPureVirtual())
      Modifiers += "pure|";
    
    if (Method->size_overridden_methods() > 0)
      Modifiers += "override|";
    
    if (Method->isStatic())
      Modifiers += "static|";
    
    if (Method->isConst())
      Modifiers += "const|";
    

    if (!Modifiers.empty())
      Modifiers.pop_back();
    
    return Modifiers;
  }


  bool VisitCXXRecordDecl(CXXRecordDecl *Declaration) {

    if (!Declaration->isCompleteDefinition() || 
        Declaration->isAnonymousStructOrUnion() ||
        Declaration->isImplicit() ||
        !Declaration->isFirstDecl() ||
        Declaration->getTemplateInstantiationPattern())
      return true;
    

    std::string className = Declaration->getNameAsString();
    

    llvm::outs() << className;
    

    if (Declaration->getNumBases() > 0) {
      llvm::outs() << " -> ";
      bool first = true;
      for (const auto &Base : Declaration->bases()) {
        if (!first) llvm::outs() << ", ";

        QualType BaseType = Base.getType();
        if (const RecordType *RT = BaseType->getAs<RecordType>()) {
          if (const CXXRecordDecl *BaseDecl = dyn_cast<CXXRecordDecl>(RT->getDecl())) {
            llvm::outs() << BaseDecl->getNameAsString();
          }
        }
        first = false;
      }
    }
    
    llvm::outs() << "\n";
    

    bool hasFields = false;
    for (const auto *Field : Declaration->fields()) {
      if (!hasFields) {
        llvm::outs() << "|_Fields\n";
        hasFields = true;
      }
      
      llvm::outs() << "| |_ " << Field->getNameAsString() << " (" 
                  << Field->getType().getAsString() << "|"
                  << getAccessSpecifierString(Field->getAccess()) << ")\n";
    }
    
    if (hasFields) {
      llvm::outs() << "|\n";
    }
    

    bool hasMethods = false;
    for (const auto *Method : Declaration->methods()) {

      if (Method->isImplicit())
        continue;
        
      if (!hasMethods) {
        llvm::outs() << "|_Methods\n";
        hasMethods = true;
      }
      

      std::string ReturnType = Method->getReturnType().getAsString();
      

      std::string Modifiers = getMethodModifiersString(Method);
      
      llvm::outs() << "| |_ " << Method->getNameAsString() << " ("
                  << ReturnType << "()" << "|"
                  << getAccessSpecifierString(Method->getAccess());
      
      if (!Modifiers.empty())
        llvm::outs() << "|" << Modifiers;
        
      llvm::outs() << ")\n";
    }
    
    llvm::outs() << "\n";
    
    return true;
  }
};


class TypeInfoConsumer : public ASTConsumer {
private:
  TypeInfoVisitor Visitor;
  
public:
  explicit TypeInfoConsumer(ASTContext *Context) : Visitor(Context) {}

  void HandleTranslationUnit(ASTContext &Context) override {

    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }
};


class TypeInfoAction : public PluginASTAction {
protected:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    return std::make_unique<TypeInfoConsumer>(&CI.getASTContext());
  }

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {
    for (unsigned i = 0, e = args.size(); i != e; ++i) {
      llvm::errs() << "TypeInfo arg = " << args[i] << "\n";
      

      if (args[i] == "help") {
        PrintHelp(llvm::errs());
        return false;
      }
    }
    return true;
  }
  
  void PrintHelp(llvm::raw_ostream& ros) {
    ros << "Помощь по плагину TypeInfo:\n";
    ros << "  Этот плагин выводит информацию о пользовательских типах данных.\n";
    ros << "  Использование: clang -cc1 -load <path-to-plugin> -plugin type-info <input-file>\n";
  }
};

} // namespace


static FrontendPluginRegistry::Add<TypeInfoAction>
X("type-info", "Вывод информации о пользовательских типах данных");
