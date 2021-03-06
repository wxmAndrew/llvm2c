#include <llvm/IR/Instruction.h>
#include "../core/Program.h"
#include "../core/Func.h"
#include "../core/Block.h"

void createAllocas(const llvm::Module* module, Program& program) {
    assert(program.isPassCompleted(PassType::CreateBlocks));

    for (const auto& function : module->functions()) {
        auto* func = program.getFunction(&function);
        for (const auto& block : function) {
            auto* myBlock = func->getBlock(&block);

            for (const auto& ins : block) {
                if (ins.getOpcode() == llvm::Instruction::Alloca) {

                    const auto allocaInst = llvm::cast<const llvm::AllocaInst>(&ins);

                    auto theVariable = std::make_unique<Value>(func->getVarName(), func->getType(allocaInst->getAllocatedType()));
                    auto alloc = std::make_unique<StackAlloc>(theVariable.get());

                    func->createExpr(&ins, std::make_unique<RefExpr>(theVariable.get(), program.typeHandler.pointerTo(theVariable.get()->getType())));

                    myBlock->addOwnership(std::move(theVariable));
                    myBlock->addExprAndOwnership(std::move(alloc));
                }
            }

        }
    }

    program.addPass(PassType::CreateAllocas);
}
