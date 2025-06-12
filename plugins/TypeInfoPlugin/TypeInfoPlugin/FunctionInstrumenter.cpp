#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

struct FunctionInstrumenter : public PassInfoMixin<FunctionInstrumenter> {
    PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
        LLVMContext &Ctx = M.getContext();
        IRBuilder<> Builder(Ctx);

        auto *Int8PtrTy = Type::getInt8Ty(Ctx)->getPointerTo();
        FunctionType *FuncTy = FunctionType::get(Type::getVoidTy(Ctx), {Int8PtrTy}, false);

        FunctionCallee StartFunc = M.getOrInsertFunction("instrument_start", FuncTy);
        FunctionCallee EndFunc   = M.getOrInsertFunction("instrument_end", FuncTy);

        for (auto &F : M) {
            if (F.isDeclaration() || F.getName().starts_with("instrument_"))
                continue;

            Constant *FuncName = Builder.CreateGlobalString(F.getName());

            BasicBlock &EntryBlock = F.getEntryBlock();
            Builder.SetInsertPoint(&EntryBlock, EntryBlock.getFirstNonPHIIt());
            Builder.CreateCall(StartFunc, {FuncName});

            for (auto &BB : F) {
                if (auto *RetInst = dyn_cast<ReturnInst>(BB.getTerminator())) {
                    Builder.SetInsertPoint(RetInst);
                    Builder.CreateCall(EndFunc, {FuncName});
                }
            }
        }

        return PreservedAnalyses::all();
    }

    static bool isRequired() { return true; }
};

}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, "FunctionInstrumenter", "v0.1",
        [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "function-instrumenter") {
                        MPM.addPass(FunctionInstrumenter());
                        return true;
                    }
                    return false;
                }
            );
        }
    };
}
