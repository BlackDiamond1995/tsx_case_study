#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/Pass.h"
#include "llvm/IR/LLVMContext.h"
#include <llvm/IR/PassManager.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/IRBuilder.h>
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include <string>
#include<fstream>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "hello"
 
namespace {
  class CFIHello : public ModulePass {
      bool isSS = true;
      map<string, Function*> wrapperMap;
      vector<string> needCallWrapperVec;
  public:
    static char ID; // Pass identification, rep lacement for typeid
    CFIHello() : ModulePass(ID) {}

    unsigned getAlignmentByType(Type *Ty)
    {
      if (Ty->isIntegerTy()) {
        return cast<IntegerType>(Ty)->getBitWidth() / 8;
      }
      if (Ty->isFloatTy()) {
        return 4;
      }
      if (Ty->isDoubleTy()) {
        return 8;
      }
      return 1;
    }

    void readFunNeedWrapperListFile(string fileName)
    {
        ifstream file(fileName);
        string line;
        if (file)
        {
            while (getline(file, line))
            {
                wrapperMap[line] = NULL;
            }
        }
        else
        {
            errs() << "No file: " << fileName << "\n";
        }
    }

    void readFunNeedCallWrapperFile(string fileName)
    {
        ifstream file(fileName);
        string line;
        if (file)
        {
            while (getline(file, line))
            {
                needCallWrapperVec.push_back(line);
            }
        }
        else
        {
            errs() << "No file: " << fileName << "\n";
        }
    }

    void paddingWrapper(Function* n_fun, Module &M, string oldFunName)
    {
        LLVMContext &ctx = M.getContext();
        //Function *n_fun = *it;
        BasicBlock* label_entry = BasicBlock::Create(ctx, "entry", n_fun, 0);
        // Get argument list
        std::vector<AllocaInst*> allocinst_list;
        for (Function::arg_iterator n_args = n_fun->arg_begin(); n_args != n_fun->arg_end(); ++n_args)
        {
            StringRef ptr_name(n_args->getName().str() + ".addr");

            AllocaInst* ptr_addr = new AllocaInst(n_args->getType(), 0, ptr_name.str(), label_entry);
            unsigned align = getAlignmentByType(n_args->getType());
            ptr_addr->setAlignment(align);
            allocinst_list.push_back(ptr_addr);
        }

        std::vector<AllocaInst*>::iterator alloc_it;
        alloc_it = allocinst_list.begin();
        for (Function::arg_iterator args = n_fun->arg_begin(); args != n_fun->arg_end(); ++args)
        {
            AllocaInst *allocinst = *alloc_it;
            StoreInst* store = new StoreInst(args, allocinst, false, label_entry);
            unsigned align = getAlignmentByType(args->getType());
            store->setAlignment(align);
            alloc_it++;
        }

        std::vector<Value*> call_params;
        alloc_it = allocinst_list.begin();
        for (Function::arg_iterator args = n_fun->arg_begin(); args != n_fun->arg_end(); ++args)
        {
            // Load
            AllocaInst *allocinst = *alloc_it;
            LoadInst* load = new LoadInst(allocinst, "", false, label_entry);
            unsigned align = getAlignmentByType(args->getType());
            load->setAlignment(align);
            call_params.push_back(load);
            alloc_it++;
        }
        Function* callee = M.getFunction(StringRef(oldFunName));
        CallInst* call = CallInst::Create(callee, call_params, "", label_entry);
        call->setCallingConv(CallingConv::C);
        call->setTailCall(false);
        AttributeList call_PAL;
        call->setAttributes(call_PAL);

        Type *ret_type = n_fun->getReturnType();
        if (!ret_type->isVoidTy())
        {
            // Return based on the return type
            ReturnInst::Create(ctx, call, label_entry);
        }
        else
        {
            // Return void
            ReturnInst::Create(ctx, label_entry);
        }
    }
    
    void makeWrapper(Module &M)
    {
        LLVMContext &ctx = M.getContext();
        int funNum = 0;
        for (auto FI = M.begin(); FI != M.end(); FI++)
        {
            funNum++;
        }
        for (auto FI = M.begin(); funNum > 0; FI++)
        {
            funNum--;
            string funName = FI->getName().str();
            if (wrapperMap.find(funName) == wrapperMap.end())
            {
                continue;
            }
            FunctionCallee nFCe = M.getOrInsertFunction(("wrapper_by_compiler_" + FI->getName().str()), FI->getFunctionType(), FI->getAttributes());
            //FunctionCallee nFCe = M.getOrInsertFunction(("wrapper_by_compiler_" + FI->getName().str()), FI->getFunctionType());
            Function* wrapperFun = cast<Function>(nFCe.getCallee()->stripPointerCasts());
            Attribute attr = Attribute::get(ctx, Attribute::NoUnwind);
            wrapperFun->addFnAttr(attr);
            //wrapperFun->stealArgumentListFrom(*FI);
            paddingWrapper(wrapperFun, M, funName);
            wrapperMap[funName] = wrapperFun;
        }
    }

    void callWrapperFirst(Module &M)
    {
        bool start = false;
        bool stop = false;
        Function* FI = M.getFunction(StringRef(needCallWrapperVec[0]));
        for(Function::iterator BBI = FI->begin(); BBI != FI->end(); ++BBI)
        {
            for(BasicBlock::iterator II = BBI->begin(); II != BBI->end(); ++II)
            {
                if(isa<CallInst>(&(*II)))
                {
                    CallInst *CI = dyn_cast<CallInst>(II);
                    if (CI->isIndirectCall())
                    {
                        continue;
                    }
                    Function *oFun = CI->getCalledFunction();
                    string oldFunName;
                    if (oFun != NULL)
                    {
                        oldFunName = oFun->getName().str();
                        
                    }
                    else
                    {
                        Value* v = CI->getCalledOperand()->stripPointerCasts();
                        oldFunName = v->getName().str();
                    }
                    if (oldFunName == "preload")
                        start = true;
                    if (oldFunName == "stop_tsx")
                        stop = true;
                    
                    if (wrapperMap.find(oldFunName) == wrapperMap.end())
                    {
                        continue;
                    }
                    if (start == true && stop == false)
                        CI->setCalledFunction(wrapperMap[oldFunName]);
                }
            }
        }
    }

    void callWrapperRest(Module &M)
    {
        for (int i = 1; i < needCallWrapperVec.size(); i++)
        {
            Function* FI = M.getFunction(StringRef(needCallWrapperVec[i]));
            for(Function::iterator BBI = FI->begin(); BBI != FI->end(); ++BBI)
            {
                for(BasicBlock::iterator II = BBI->begin(); II != BBI->end(); ++II)
                {
                    if(isa<CallInst>(&(*II)))
                    {
                        CallInst *CI = dyn_cast<CallInst>(II);
                        if (CI->isIndirectCall())
                        {
                            continue;
                        }
                        Function *oFun = CI->getCalledFunction();
                        string oldFunName;
                        if (oFun != NULL)
                        {
                            oldFunName = oFun->getName().str();
                            
                        }
                        else
                        {
                            Value* v = CI->getCalledOperand()->stripPointerCasts();
                            oldFunName = v->getName().str();
                        }

                        if (wrapperMap.find(oldFunName) == wrapperMap.end())
                        {
                            continue;
                        }
                        CI->setCalledFunction(wrapperMap[oldFunName]);
                    }
                }
            }
        }
    }

    void callWrapper(Module &M)
    {
        for(Module::iterator FI = M.begin(); FI != M.end(); ++FI)
        {
            if (FI->getName().str().find("wrapper_by_compiler_") != string::npos)
            {
                continue;
            }
            for(Function::iterator BBI = FI->begin(); BBI != FI->end(); ++BBI)
            {
                for(BasicBlock::iterator II = BBI->begin(); II != BBI->end(); ++II)
                {
                    if(isa<CallInst>(&(*II)))
                    {
                        CallInst *CI = dyn_cast<CallInst>(II);
                        if (CI->isIndirectCall())
                        {
                            continue;
                        }
                        Function *oFun = CI->getCalledFunction();
                        string oldFunName;
                        if (oFun != NULL)
                        {
                            oldFunName = oFun->getName().str();
                            
                        }
                        else
                        {
                            Value* v = CI->getCalledOperand()->stripPointerCasts();
                            oldFunName = v->getName().str();
                        }

                        if (wrapperMap.find(oldFunName) == wrapperMap.end())
                        {
                            continue;
                        }
                        CI->setCalledFunction(wrapperMap[oldFunName]);
                    }
                }
            }
        }
    }

    bool runOnModule(Module &M) override {
        LLVMContext &ctx = M.getContext();
        FunctionCallee funDeclareTransactionBegin = M.getOrInsertFunction("transactionBegin", Type::getVoidTy(ctx));
        FunctionCallee funDeclareTransactionEndBegin = M.getOrInsertFunction("transactionEndBegin", Type::getVoidTy(ctx));
        FunctionCallee funDeclareReload = M.getOrInsertFunction("reload", Type::getVoidTy(ctx), Type::getInt8PtrTy(ctx));
        FunctionCallee funDeclareExit = M.getOrInsertFunction("exit", Type::getVoidTy(ctx), Type::getInt32Ty(ctx));
        M.getOrInsertGlobal("ssStart", Type::getInt64Ty(ctx));
        // Module::iterator FI = M.begin();
        // Function::iterator BBI = FI->begin();
        // Instruction &I = BBI->front();
        // Instruction *inst = &I;
        // IRBuilder<> builder(inst);
        // Value *formatStr = builder.CreateGlobalStringPtr("analysis", ".mystr");
        // builder.createint
        readFunNeedWrapperListFile("fun_need_wrapper_list");
        readFunNeedCallWrapperFile("fun_need_call_wrapper");
        makeWrapper(M);
        if (isSS == true)
        {
            callWrapper(M);
        }
        else
        {
            callWrapperFirst(M);
            callWrapperRest(M);
        }
        return 1;
    }
  };
}
 
char CFIHello::ID = 0;
static RegisterPass<CFIHello> X("cfihello", "CFI Pass");

static void registerCFIHello(const PassManagerBuilder &,
                                llvm::legacy::PassManagerBase &PM) {
       PM.add(new CFIHello());
}

static RegisterStandardPasses
    RegisterCFIHello0(PassManagerBuilder::EP_EnabledOnOptLevel0,
                          registerCFIHello);

static RegisterStandardPasses
    RegisterCFIHello(PassManagerBuilder::EP_ModuleOptimizerEarly,
                          registerCFIHello);