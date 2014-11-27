#include "Executor.h"

#include <cassert>
#include <queue>
#include <vector>

#include "../commons.h"
#include "../Data.cpp"
#include "../instructions/Instruction.h"
#include "../instructions/InstructionBuilder.cpp"
#include "../instructions/RawInstruction.h"

unsigned int Executor::executionCycle = 0;

void Executor::instFetchStage() {
	// Read memory for nextPC.
	// Add instruction to instructionQueue
	Data* data = getMemoryData(nextPC);
	instructionQueue.push(new RawInstruction(data, getExecutionCycle() + 1));

	// Check BTB for next address
	// update nextPC
	nextPC = btb->getNextPC(nextPC);
}

void Executor::decodeStage() {
	// Read instructionQueue. If empty do nothing
	if (instructionQueue.size() == 0) {
		return;
	}

	// If front most inst scheduled for future cycle, then do nothing
	RawInstruction* rawInst = instructionQueue.front();
	if (rawInst->getDecodeCycle() > executionCycle) {
		return;
	}

	// Else decode Instruction using instructionBuilder.
	Instruction* instruction = InstructionBuilder::build(rawInst->getAddress(),
			rawInst->getBitString(), executionCycle + 1);

	INSTRUCTIONS opCode = instruction->getOpCode();

	// Check ROB and RS. If not empty, do nothing
	// Extra slot in ROB for saving address of the load and store instruction??
	if (!rob->isFull() && (opCode == NOP || opCode == BREAK)) {
		// IN NOP or BREAK and slot available in ROB, simply add
		ROBSlot* slotEntry = rob->queueInstruction(instruction);
		instruction->setROBSlot(slotEntry);
	} else if (!rob->isFull() && !resStation->isFull()) {
		// Else ensure space in both RS and ROB
		ROBSlot* slotEntry = rob->queueInstruction(instruction);
		instruction->setROBSlot(slotEntry);

		// Add entry to RS. Update arguments from register or ROB. (Qj, Qk,Vj, Vk)
		RSEntry* rsEntry = resStation->add(instruction);
		instruction->setRSId(rsEntry->getRSId());
		rsEntry->setROBSlot(slotEntry);

		// If inst writes to register, update ROBId in registerStatus
		if (instruction->writesToRegister()) {
			regStatus->set(instruction->getDestination(),
					slotEntry->getIndex());
		}

	} else {
		// Else skip cycle
		return;
	}
}

void Executor::executeStage() {
	int aluUsed = 0;

	vector<RSEntry*> reservations = resStation->getEntries();

	// Calculate memory address of LW\SW which is ahead in the queue
	RSEntry* minLoadStoreEntry = NULL;

	for (vector<RSEntry*>::iterator it; it != reservations.end(); ++it) {
		RSEntry* rsEntry = *it;
		Instruction* instruction = rsEntry->getInstruction();
		INSTRUCTIONS opcode = instruction->getOpCode();

		if (instruction->getExecuteCyclesLeft() > 0 && rsEntry->getQj() == 0
				&& rsEntry->getQk() == 0) {
			if (instruction->isBranchInst() || opcode == J) {
				bool outcome = instruction->outcome(rsEntry->getVj(),
						rsEntry->getVk());

				unsigned int newAddress = instruction->getDestination();

				//Update BTB
				btb->updateOrAdd(instruction->getAddress(), newAddress,
						outcome);

				//Update ROB with the destination and branch outcome
				rsEntry->getROBSlot()->setDestination(newAddress);
				rsEntry->getROBSlot()->setBranchOutcome(outcome);

				//Mark the instruction ready
				rsEntry->getROBSlot()->makeReady();
				instruction->decrementExecuteCyclesLeft();
				aluUsed++;

			} else if (opcode != BREAK && opcode != NOP && opcode != SW
					&& opcode != LW) {
				//Write the result to CDB
				cdb->set(instruction->getROBSlot()->getIndex(),
						instruction->execute(rsEntry->getVj(),
								rsEntry->getVk()));
				instruction->decrementExecuteCyclesLeft();
				aluUsed++;

			} else if (opcode == SW || opcode == LW) {
				unsigned int executionCyclesLeft =
						instruction->getExecuteCyclesLeft();
				//Memory address will be calculated in the first cycle
				if (executionCyclesLeft == 2) {
					if (!minLoadStoreEntry) {
						minLoadStoreEntry = rsEntry;
					} else if (minLoadStoreEntry->getInstruction()->getExecutionCycle()
							< instruction->getExecutionCycle()) {
						minLoadStoreEntry = rsEntry;
					}
				} else if (executionCyclesLeft == 1) {
					if (opcode == SW) {
						if (rsEntry->getQk() == 0) {
							instruction->decrementExecuteCyclesLeft();
							rsEntry->getROBSlot()->makeReady();
						}
					} else {
						int earlyStorePresent = false;
						for (vector<RSEntry*>::iterator it;
								it != reservations.end(); ++it) {
							RSEntry* rsEntryTemp = *it;
							Instruction* instructionTemp =
									rsEntry->getInstruction();
							INSTRUCTIONS opcodeTemp = instruction->getOpCode();

							if (opcodeTemp == SW
									&& instructionTemp->getExecutionCycle()
											< instruction->getExecutionCycle()
									&& rsEntryTemp->getAddress()
											== rsEntry->getAddress()) {
								earlyStorePresent = true;
								break;
							}
						}

						if (!earlyStorePresent) {
							cdb->set(instruction->getROBSlot()->getIndex(),
									getMemoryData(rsEntry->getAddress())->getValue());
							instruction->decrementExecuteCyclesLeft();
						}
					}
				}
			}
		}
	}

	// Address should be calculated for The LW\SW entry at the head of the queue.
	if (minLoadStoreEntry) {
		minLoadStoreEntry->setAddress(
				minLoadStoreEntry->getInstruction()->execute(
						minLoadStoreEntry->getVj(),
						minLoadStoreEntry->getVk()));

		minLoadStoreEntry->getInstruction()->decrementExecuteCyclesLeft();
	}

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
// When result available in CDB
//      update ROB
//      make ROB instruction ready to commit in next cycle.
	rob->updateFromCDB();

//      update RS.
//  If Load or store, do the following
//  Update the ROBSlot with destination.
//  Update RS with the caculated address
//  RS also updates SWAddToCount map to handle load stall.
	resStation->updateFromCDB();

// Flush CDB at the end of this cycle
	cdb->flush();
}

void Executor::commitStage() {
	if (rob->isEmpty()) {
		return;
	}

// Check if commit is scheduled in this cycle
	ROBSlot* slot = rob->peekTop();
	Instruction* inst = slot->getInstruction();
// If not scheduled in this cycle or not ready
	if (!slot->isReady() || inst->getExecutionCycle() > executionCycle) {
		return;
	}

// Commit is in order. One instruction from top of the queue.
	rob->dequeueInstruction();

	unsigned int destination = slot->getDestination();
	INSTRUCTIONS opCode = inst->getOpCode();

// Branching:
//      If predicted correctly, commit and remove from ROB and RS
//      If wrongly predicted, update PC accordingly and clear all instruction after that stage. Commit branche and remove branch from ROB and RS.
//      For jump same, if not present in BTB, update BTB and discard all following instructions.
//      PC to be updated will be nextPC.
	if (opCode == J || inst->isBranchInst()) {
		// check the ROB's next instruction's address
		// is same as destination
		ROBSlot* nextSlot = rob->peekTop();
		Instruction* nextInst = nextSlot->getInstruction();

		// if not same,
		if (destination != nextInst->getAddress()) {
			// then flush the whole system
			// (ROB, RS, IQ, Register Status)
			flush();

			// and update nextPC;
			nextPC = destination;
		}

	} else if (opCode == SW) {
		// update destination memory with value
		memory[destination] = new Data(destination, slot->getValue());

	} else {
		// For ALU  and load instruction, register is updated with the result
		registers[destination] = slot->getValue();
	}

// finally vacate the reservation station.
	int RSId = inst->getRSId();
	assert(RSId > -1);
	resStation->remove(RSId);
}

void Executor::run() {

// TODO: Add stop condition
	while (true) {
		executionCycle++;

		instFetchStage();
		decodeStage();
		executeStage();
		writeResultStage();
		commitStage();
	}
}

void Executor::flush() {
	rob->flush();
	resStation->flush();
	regStatus->flush();
	cdb->flush();
}
