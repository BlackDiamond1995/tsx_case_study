#include "X86.h"
#include "X86InstrBuilder.h"
#include "X86InstrInfo.h"
#include "X86Subtarget.h"
#include "llvm/Pass.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/LiveVariables.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/MCContext.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace llvm;
using namespace X86;
using namespace std;

//打印MachineInstr的相关信息，IorB用于表示该MI是原有的还是插入的
void printMachineInstr(MachineInstr &MI)
{
    MI.print(outs());
    for (int i = 0; i < MI.getNumOperands(); i++)
    {
        MachineOperand MO = MI.getOperand(i);
        //outs() << "\t" << MI.getOperand(i) << "\t" <<MO.isReg() << "\t" << MO.isImm() << "\n";
        outs() << " " << MI.getOperand(i) << " isBlockAddress " << MO.isBlockAddress() << " isCFIIndex " << MO.isCFIIndex() << " isCImm " << MO.isCImm() << " isCPI " << MO.isCPI() << " isFI " << MO.isFI() << " isFPImm " << MO.isFPImm() << " isGlobal " << MO.isGlobal() << " isImm " << MO.isImm() << " isIntrinsicID " << MO.isIntrinsicID() << " isJTI " << MO.isJTI() << " isMBB " << MO.isMBB() << " isMCSymbol " << MO.isMCSymbol() << " isMetadata " << MO.isMetadata() << " isPredicate " << MO.isPredicate() << " isReg " << MO.isReg() << " isRegLiveOut " << MO.isRegLiveOut() << " isRegMask " << MO.isRegMask() << " isSymbol " << MO.isSymbol() << " isTargetIndex " << MO.isTargetIndex()<< "\n";
    }
    outs() << "\n";
}

namespace {
	class X86ShadowStackCFIPass : public MachineFunctionPass {	
    private:
        bool isSS = false;
        const GlobalValue* targetfileNameStrGV;
        const GlobalValue* ssStartGV;
        const GlobalValue* reloadGV;
        const GlobalValue* transactionBeginGV;
        const GlobalValue* transactionEndBeginGV;
        const GlobalValue* exitGV;
        bool hasGV = false;
        vector<string> needTsxVec;
    public:
        static char ID;
        X86ShadowStackCFIPass() : MachineFunctionPass(ID) {}

                //在函数入口进行ShadowStack插桩
        void insertSSEntry(const TargetInstrInfo *TII, MachineBasicBlock &MBB, const DebugLoc &DL)
        { 
            auto MII = MBB.begin();
            
            //movq [%rsp], %r15
            addDirectMem(BuildMI(MBB, MII, DL, TII->get(X86::MOV64rm)).addDef(X86::R15), X86::RSP);
            if (MBB.getParent()->getFunction().getName().str() != "main")
                //xend
                BuildMI(MBB, MII, DL, TII->get(X86::XEND));
            // movq ssStart@GOTPCREL(%rip), %r11
            MachineInstr *tmpMI = BuildMI(MBB, MII, DL, TII->get(X86::MOV64rm)).addReg(X86::R11).addReg(X86::RIP).addImm(1).addReg(0).addGlobalAddress(ssStartGV).addReg(0);
            tmpMI->getOperand(4).addTargetFlag(X86II::MO_GOTPCREL);
            // addq 8, [%r11]
            BuildMI(MBB, MII, DL, TII->get(X86::ADD64mi8)).addReg(X86::R11).addImm(1).addReg(0).addImm(0).addReg(0).addImm(8);
            // movq [%r11], %r10
            addDirectMem(BuildMI(MBB, MII, DL, TII->get(X86::MOV64rm)).addDef(X86::R10), X86::R11);
            // addq %r10, %r11
            BuildMI(MBB, MII, DL, TII->get(X86::ADD64rr)).addReg(X86::R11).addDef(X86::R11).addReg(X86::R10);
            // //movq %r15, %r10
            // BuildMI(MBB, MII, DL, TII->get(X86::MOV64rr)).addReg(X86::R10).addReg(X86::R15);
            // movq %r15, [%r11]
            BuildMI(MBB, MII, DL, TII->get(X86::MOV64mr)).addReg(X86::R11).addImm(1).addReg(0).addImm(0).addReg(0).addReg(X86::R15);
        }

        //在函数出口进行ShadowStack插桩
        void insertSSRet(const TargetInstrInfo *TII, MachineBasicBlock &MBB, MachineInstr &MI, MachineBasicBlock &TrapBB)
        {
            const DebugLoc &DL = MI.getDebugLoc();
            // movq ssStart@GOTPCREL(%rip), %r11
            MachineInstr *tmpMI = BuildMI(MBB, MI, DL, TII->get(X86::MOV64rm)).addReg(X86::R11).addReg(X86::RIP).addImm(1).addReg(0).addGlobalAddress(ssStartGV).addReg(0);
            tmpMI->getOperand(4).addTargetFlag(X86II::MO_GOTPCREL);
            // mov [%r11], %r10
            addDirectMem(BuildMI(MBB, MI, DL, TII->get(X86::MOV64rm)).addDef(X86::R10), X86::R11);
            // add %r11, %r10
            BuildMI(MBB, MI, DL, TII->get(X86::ADD64rr)).addReg(X86::R10).addDef(X86::R10).addReg(X86::R11);
            // sub 8, [%r11]
            BuildMI(MBB, MI, DL, TII->get(X86::SUB64mi8)).addReg(X86::R11).addImm(1).addReg(0).addImm(0).addReg(0).addImm(8);
            // mov [%r10], %r11
            addDirectMem(BuildMI(MBB, MI, DL, TII->get(X86::MOV64rm)).addDef(X86::R11), X86::R10);
            // // movq %rax, %r15
            // BuildMI(MBB, MI, DL, TII->get(X86::MOV64rr)).addReg(X86::R15).addReg(X86::RAX);
            //pushq %rax
            BuildMI(MBB, MI, DL, TII->get(X86::PUSH64r)).addReg(X86::RAX);
            //callq transactionBegin
            BuildMI(MBB, MI, DL, TII->get(X86::CALL64pcrel32)).addGlobalAddress(transactionBeginGV);
            //popq %rax
            BuildMI(MBB, MI, DL, TII->get(X86::POP64r)).addReg(X86::RAX);
            // //movq %r15, %rax
            // BuildMI(MBB, MI, DL, TII->get(X86::MOV64rr)).addReg(X86::RAX).addReg(X86::R15);
            // cmp %r11, [%rsp]
            addDirectMem(BuildMI(MBB, MI, DL, TII->get(X86::CMP64mr)), X86::RSP).addReg(X86::R11);
            // jne trap
            BuildMI(MBB, MI, DL, TII->get(X86::JCC_1)).addMBB(&TrapBB).addImm(5);
            if (MBB.getParent()->getFunction().getName().str() == "main")
                //xend
                BuildMI(MBB, MI, DL, TII->get(X86::XEND));
            MBB.addSuccessorWithoutProb(&TrapBB);
        }

        //检查函数能否进行shadowstack插桩
        bool checkSSMF(MachineFunction &MF)
        {
            if (MF.empty() || !MF.getRegInfo().tracksLiveness())
                return false;
            if (MF.front().isLiveIn(X86::R10) || MF.front().isLiveIn(X86::R11) || MF.front().isLiveIn(X86::R15))
                return false;
            
            bool HasReturn = false;
            for (auto &MBB : MF)
            {
                if (MBB.empty())
                    continue;
                const MachineInstr &MI = MBB.instr_back();
                if (MI.isReturn())
                {
                    if (MI.findRegisterUseOperand(X86::R11))
                        return false;
                    if (MI.findRegisterUseOperand(X86::R10))
                        return false;
                    if (MI.findRegisterUseOperand(X86::R15))
                        return false;
                    HasReturn = true;
                }
                if (MI.isReturn() && MI.isCall())
                {
                    if (MI.findRegisterUseOperand(X86::EFLAGS))
                        HasReturn = false;
                    if (MI.findRegisterUseOperand(X86::R10) || MI.hasRegisterImplicitUseOperand(X86::R10) || MI.hasRegisterImplicitUseOperand(X86::R15))
                        HasReturn = false;
                }
            }
            return HasReturn;
        }

        //插入shadowstack指令
        bool insertShadowStackInst(MachineFunction &MF)
        {
            if (!checkSSMF(MF))
                return false;

            MachineBasicBlock &MBB = MF.front();
            const MachineBasicBlock *NonEmpty = MBB.empty() ? MBB.getFallThrough() : &MBB;
            const DebugLoc &DL = NonEmpty->front().getDebugLoc();

            const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
            insertSSEntry(TII, MBB, DL);

            MachineBasicBlock *Trap = nullptr;
            for (auto &MBB : MF)
            {
                if (MBB.empty())
                    continue;
                auto MII = MBB.end();
                MII--;
                if (MII->isReturn())
                {
                    Trap = MF.CreateMachineBasicBlock();
                    Trap->setLabelMustBeEmitted();
                    //xend
                    BuildMI(Trap, MII->getDebugLoc(), TII->get(X86::XEND));
                    // callq exit(-1)
                    BuildMI(Trap, MII->getDebugLoc(), TII->get(X86::MOV32ri)).addReg(EDI).addImm(0xFFFFFFFF);
                    BuildMI(Trap, MII->getDebugLoc(), TII->get(X86::CALL64pcrel32)).addGlobalAddress(exitGV);
                    MF.push_back(Trap);
                    MachineInstr &MI = *MII;
                    insertSSRet(TII, MBB, MI, *Trap);
                    return true;
                }
            }
        }

        bool ssTsxInsert(MachineFunction &MF)
        {
            string funName = MF.getFunction().getName().str();
            if (funName.find("wrapper_by_compiler_") != string::npos)
            {
                return false;
            }
            const TargetInstrInfo *TII;
            TII = MF.getSubtarget().getInstrInfo();
            MachineBasicBlock &MBBf = MF.front();
            const MachineBasicBlock *NonEmpty = MBBf.empty() ? MBBf.getFallThrough() : &MBBf;
            const DebugLoc &DL = NonEmpty->front().getDebugLoc();
            for (auto MBBI = MF.begin(); MBBI != MF.end(); MBBI++)
            {
                for (auto MII = MBBI->begin(); MII != MBBI->end(); MII++)
                {
                    if (MII->isCall())
                    {
                        // // movq %rax, %r15
                        // BuildMI(*MBBI, *MII, DL, TII->get(X86::MOV64rr)).addReg(X86::R15).addReg(X86::RAX);
                        //pushq %rax
                        BuildMI(*MBBI, *MII, DL, TII->get(X86::PUSH64r)).addReg(X86::RAX);
                        //callq transactionBegin
                        BuildMI(*MBBI, *MII, DL, TII->get(X86::CALL64pcrel32)).addGlobalAddress(transactionBeginGV);
                        //popq %rax
                        BuildMI(*MBBI, *MII, DL, TII->get(X86::POP64r)).addReg(X86::RAX);
                        // //movq %r15, %rax
                        // BuildMI(*MBBI, *MII, DL, TII->get(X86::MOV64rr)).addReg(X86::RAX).addReg(X86::R15);
                        MII++;
                        if (MII == MBBI->end())
                        {
                            MII--;
                            if(MII->isBranch() && MII->getOperand(0).isMBB())
                            {
                                MachineBasicBlock* MBBSI = MII->getOperand(0).getMBB();
                                MBBSI->setLabelMustBeEmitted();
                            }
                            MII++;

                            //xend
                            BuildMI(*MBBI, MBBI->end(), DL, TII->get(X86::XEND));
                        }
                        else
                        {
                            //xend
                            BuildMI(*MBBI, *MII, DL, TII->get(X86::XEND));
                        }
                        MII--;
                    }
                    else
                    {
                        continue;
                    }
                }
            }
            return true;
        }

        void inserTransactionCall(MachineBasicBlock &MBB, MachineInstr &MI, const GlobalValue* transactionCallGV)
        {
            const TargetInstrInfo *TII;
            TII = MBB.getParent()->getSubtarget().getInstrInfo();
            const DebugLoc &DL = MBB.front().getDebugLoc();
            //pushq rax
            BuildMI(MBB, MI, DL, TII->get(PUSH64r)).addReg(X86::RAX);
            //callq transactionBegin
            BuildMI(MBB, MI, DL, TII->get(X86::CALL64pcrel32)).addGlobalAddress(transactionCallGV);
            //popq %rax
            BuildMI(MBB, MI, DL, TII->get(POP64r)).addReg(X86::RAX);
        }

        bool wrapperTsxInsert(MachineFunction &MF)
        {
            const TargetInstrInfo *TII;
            TII = MF.getSubtarget().getInstrInfo();
            MachineBasicBlock &MBBf = MF.front();
            const MachineBasicBlock *NonEmpty = MBBf.empty() ? MBBf.getFallThrough() : &MBBf;
            const DebugLoc &DL = NonEmpty->front().getDebugLoc();
            auto MBBI = MF.begin();

            //XEND
            BuildMI(*MBBI, MBBI->begin(), DL, TII->get(X86::XEND));
            
            auto MII = MBBI->end();
            MII--;
            //pushq rax
            BuildMI(*MBBI, *MII, DL, TII->get(PUSH64r)).addReg(X86::RAX);
            // //movq %rax, %r15
            // BuildMI(*MBBI, *MII, DL, TII->get(X86::MOV64rr)).addReg(X86::R15).addReg(RAX);
            //leaq targetfileNameStrGV+%rip, %rdi
            BuildMI(*MBBI, *MII, DL, TII->get(X86::LEA64r)).addReg(X86::RDI).addReg(X86::RIP).addImm(1).addReg(0).addGlobalAddress(targetfileNameStrGV).addReg(0);
            //callq reload
            BuildMI(*MBBI, *MII, DL, TII->get(X86::CALL64pcrel32)).addGlobalAddress(reloadGV);
            //callq transactionBegin
            BuildMI(*MBBI, *MII, DL, TII->get(X86::CALL64pcrel32)).addGlobalAddress(transactionBeginGV);
            //popq rax
            BuildMI(*MBBI, *MII, DL, TII->get(POP64r)).addReg(X86::RAX);
            // //movq %r15, %rax
            // BuildMI(*MBBI, *MII, DL, TII->get(X86::MOV64rr)).addReg(X86::RAX).addReg(R15);
        }

        bool restTsxInsert(MachineFunction &MF)
        {
            string funName = MF.getFunction().getName().str();
            const TargetInstrInfo *TII;
            TII = MF.getSubtarget().getInstrInfo();
            MachineBasicBlock &MBBf = MF.front();
            const MachineBasicBlock *NonEmpty = MBBf.empty() ? MBBf.getFallThrough() : &MBBf;
            const DebugLoc &DL = NonEmpty->front().getDebugLoc();
            for (auto MBBI = MF.begin(); MBBI != MF.end(); MBBI++)
            {
                //popq rax
                BuildMI(*MBBI, MBBI->begin(), DL, TII->get(X86::POP64r)).addReg(X86::RAX);
                //callq transactionEndBegin
                BuildMI(*MBBI, MBBI->begin(), DL, TII->get(X86::CALL64pcrel32)).addGlobalAddress(transactionEndBeginGV);
                //pushq rax
                BuildMI(*MBBI, MBBI->begin(), DL, TII->get(X86::PUSH64r)).addReg(X86::RAX);
            }
            return true;
        }
        
        bool firstTsxInsert(MachineFunction &MF)
        {
            bool start = false;
            bool stop = false;
            const TargetInstrInfo *TII;
            TII = MF.getSubtarget().getInstrInfo();
            MachineBasicBlock &MBBf = MF.front();
            const MachineBasicBlock *NonEmpty = MBBf.empty() ? MBBf.getFallThrough() : &MBBf;
            const DebugLoc &DL = NonEmpty->front().getDebugLoc();
            for (auto MBBI = MF.begin(); MBBI != MF.end(); MBBI++)
            {
                if (start == true && stop == false)
                {
                    //popq rax
                    BuildMI(*MBBI, MBBI->begin(), DL, TII->get(X86::POP64r)).addReg(X86::RAX);
                    //callq transactionEndBegin
                    BuildMI(*MBBI, MBBI->begin(), DL, TII->get(X86::CALL64pcrel32)).addGlobalAddress(transactionEndBeginGV);
                    //pushq rax
                    BuildMI(*MBBI, MBBI->begin(), DL, TII->get(X86::PUSH64r)).addReg(X86::RAX);
                }
                // else if (start == true && MBBI->isReturnBlock())
                // {
                //     auto MII = MBBI->end();
                //     MII--;
                //     BuildMI(*MBBI,*MII, DL, TII->get(X86::XEND));
                // }
                for (auto MII = MBBI->begin(); MII != MBBI->end(); MII++)
                {
                    if (MII->getOpcode() == X86::CALL64pcrel32 && MII->getOperand(0).getGlobal()->getName().str() == "preload")
                    {
                        start = true;
                        MII++;
                        if (MII != MBBI->end())
                        {
                            //pushq rax
                            BuildMI(*MBBI, *MII, DL, TII->get(PUSH64r)).addReg(X86::RAX);
                            //callq transactionBegin
                            BuildMI(*MBBI, *MII, DL, TII->get(X86::CALL64pcrel32)).addGlobalAddress(transactionBeginGV);
                            //popq rax
                            BuildMI(*MBBI, *MII, DL, TII->get(POP64r)).addReg(X86::RAX);                           
                        }
                        else
                        {
                            //pushq rax
                            BuildMI(*MBBI, MBBI->end(), DL, TII->get(PUSH64r)).addReg(X86::RAX);
                            //callq transactionBegin
                            BuildMI(*MBBI, MBBI->end(), DL, TII->get(X86::CALL64pcrel32)).addGlobalAddress(transactionBeginGV);
                            //popq rax
                            BuildMI(*MBBI, MBBI->end(), DL, TII->get(POP64r)).addReg(X86::RAX);
                            break;
                        }
                    }
                    if (MII->getOpcode() == X86::CALL64pcrel32 && MII->getOperand(0).getGlobal()->getName().str() == "stop_tsx")
                    {
                        stop = true;
                        BuildMI(*MBBI, *MII, DL, TII->get(X86::XEND));
                    }
                }
            }
            return true;
        }

        bool tsxInsert(MachineFunction &MF)
        {
            string funName = MF.getFunction().getName().str();
            if (funName.find("wrapper_by_compiler_") != string::npos)
            {
                wrapperTsxInsert(MF);
            }
            else if (find(needTsxVec.begin(), needTsxVec.end(), funName) == needTsxVec.begin())
            {
                firstTsxInsert(MF);
            }
            else if (find(needTsxVec.begin(), needTsxVec.end(), funName) != needTsxVec.end())
            {
                restTsxInsert(MF);
            }
        }

        void getGlobalAddress(MachineFunction &MF)
        {
            const Function &F = MF.getFunction();
            const Module &M = *F.getParent();
            ssStartGV = M.getNamedValue("ssStart");
            targetfileNameStrGV = M.getNamedValue("preloadStr");
            reloadGV = M.getNamedValue("reload");
            transactionBeginGV = M.getNamedValue("transactionBegin");
            transactionEndBeginGV = M.getNamedValue("transactionEndBegin");
            exitGV = M.getNamedValue("exit");
            hasGV = true;
        }

        void readFunNeedInstrument(string fileName)
        {
            ifstream file(fileName);
            string line;
            if (file)
            {
                while (getline(file, line))
                {
                    needTsxVec.push_back(line);
                }
            }
            else
            {
                errs() << "No file: " << fileName << "\n";
            }
        }

        virtual bool runOnMachineFunction(MachineFunction &MF) {
            if (hasGV == false)
                getGlobalAddress(MF);
            if (needTsxVec.size() == 0)
                readFunNeedInstrument("fun_need_call_wrapper");
            if (isSS == true)
            {
                ssTsxInsert(MF);
                insertShadowStackInst(MF);
            }
            else
            {
                tsxInsert(MF);
            }
            // for (auto MBBI = MF.begin(); MBBI != MF.end(); MBBI++)
            // {
            //     for (auto MII = MBBI->begin(); MII != MBBI->end(); MII++)
            //     {
            //         //if (MII->readsWritesVirtualRegister(X86::RCX).second)
            //         if (MII->getOpcode() == X86::MOV64ri)
            //         {
            //             MachineInstr &MI = *MII;
            //             printMachineInstr(MI);
            //         }
            //     }
            // }
            return false;
        }
    };
}

namespace llvm {
FunctionPass *createX86ShadowStackCFIPass(){
    return new X86ShadowStackCFIPass();
}
}

char X86ShadowStackCFIPass::ID = 0;
static RegisterPass<X86ShadowStackCFIPass> X("X86ShadowStackCFIPass", "X86ShadowStackCFIPass");