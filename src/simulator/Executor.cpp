#include "Executor.h"


void Executor::instFetchStage() {
    // Read memory for nextPC.
    // Add instruction to instructionQueue
    // Check BTB for next address
    // update nextPC
}


void Executor::decodeStage() {
    // Read instructionQueue. If empty do nothing
    // Else decode Instruction using instructionBuilder.
    // Check ROB and RS. If not empty, do nothing
    // Else add to ROB. Update instruction with ROB address for returned value.
    // Add entry to RS. Update arguments from register or ROB. (Qj, Qk,Vj, Vk)
}


void Executor::executeStage() {
    // Execute if both operands available.
    // All instructions take 1 cycle.
    // Load/Store take 2. One for address computation.
    // One for memory access. That is read memory in cycle 2.
    // 10 execution units so instructions can be executed in parallel.

    // Branch jump resolved in this stage.
    // Save branch outcome and target in ROB and branch/jump are marked ready.
    // Update BTB with target address and 1 bit predictor.


    // Load and store are executed in order
    // Load's Two steps:
    //      1. Address Calculation
    //          Ensure all previous load/store have their address ready in ROB before the execution.
    //      2. Memory Access:
    //          Two cases:
    //              i. No early store in ROB on same address, go ahead and read.
    //              ii. Matched  store in ROB. Wait for store to commit and that triggers the load.

    // Store
    //      Two steps:
    //          1. Address Calculation
    //              When base register is ready, all load and store ahead have address ready in ROB, calculate address in 1 cycle.
    //          2. Store
    //              Happens in commit stage.
    //              Store marked ready in ROB when address and data ready.
    //              Wait in RS for AC and data computation.
    //              Once both available store marked ready to commit.


    // Reading CDB done in this stage.
}


void Executor::writeResultStage() {
    // When result available, write it into CDB and from there to ROB and RS.
    // Store, Branch. NOP and BREAK will skip this step.
    // This step also update values in ROB and make ROB instruction ready to commit.


    /* Figure out handling of load and store in write result stage */
}


void Executor::commitStage() {
    // Commit is in order. One instruction from top of the queue.
    //
    // For ALU instruction, register is updated with the result and instruction removed from ROB and RS.
    // *** Doesn't make sense ***  Load commit is same, just that results written to memory.
    // Store updates memory. Any dependent load is wakened up and does memory access in next cycle.


    // Branching:
    //      If predicted correctly, commit and remove from ROB and RS
    //      If wrongly predicted, update PC accordingly and clear all instruction after that stage. Commit branche and remove branch from ROB and RS.
    //      For jump same, if not present in BTB, update BTB and discard all following instructions.
    //      PC to be updated will be nextPC.
}

void Executor::run() {
        cout << "Executor Running";
}
