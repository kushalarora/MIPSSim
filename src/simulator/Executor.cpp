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

    if (nextPC > codeSegmentEnd) {
        return;
    }

	Data* data = getMemoryData(nextPC);

    Instruction* inst = InstructionBuilder::build(nextPC, data->getBitString());

    if (inst->getOpCode() == BREAK) {
        codeSegmentEnd = nextPC;
    }
    RawInstruction* rawInst = new RawInstruction(data, getExecutionCycle() + 1);
	instructionQueue.push_back(rawInst);
	cout << "	Pushed Instruction at address: " << data->getAddress()<<endl;
	// Check BTB for next address
	// update nextPC
    if (inst->getOpCode() == J || inst->isBranchInst()) {
        nextPC = btb->getNextPC(nextPC, inst->getDestination());
        rawInst->setOutCome(nextPC == inst->getDestination());
        cout << "Branch Fetch Outcome: " << rawInst->getOutCome() << endl;
    } else {
        nextPC = nextPC + 4;
    }
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
	instruction->setOutCome(rawInst->getOutCome());
	cout << "	Decode Instruction: " << instruction->instructionString()<<endl;

	INSTRUCTIONS opCode = instruction->getOpCode();

	// Check ROB and RS. If not empty, do nothing
	// Extra slot in ROB for saving address of the load and store instruction??
	if (!rob->isFull() && (opCode == NOP || opCode == BREAK)) {

		// IN NOP or BREAK and slot available in ROB, simply add
		ROBSlot* slotEntry = rob->queueInstruction(instruction);
		instruction->setROBSlot(slotEntry);
        slotEntry->makeReady();
		instructionQueue.pop_front();

	} else if ((!rob->isFull()) && (!resStation->isFull())) {

		// Else ensure space in both RS and ROB
		ROBSlot* slotEntry = rob->queueInstruction(instruction);
		instruction->setROBSlot(slotEntry);

		// Add entry to RS. Update arguments from register or ROB. (Qj, Qk,Vj, Vk)
		RSEntry* rsEntry = resStation->add(instruction);
		instruction->setRSId(rsEntry->getRSId());

		// If inst writes to register, update ROBId in registerStatus
		if (instruction->writesToRegister()) {
			regStatus->set(instruction->getDestination(),
					slotEntry->getIndex());
		}
		instructionQueue.pop_front();

	}
}

void Executor::executeStage() {
	int aluUsed = 0;

	vector<RSEntry*>& reservations = resStation->getEntries();

	// Calculate memory address of LW\SW which is ahead in the queue
	RSEntry* minLoadStoreEntry = NULL;

	for (vector<RSEntry*>::iterator it = reservations.begin(); it != reservations.end(); ++it) {
		RSEntry* rsEntry = *it;
		Instruction* instruction = rsEntry->getInstruction();

        // If scheduled for future cycles, just ignore
        if (instruction->getExecutionCycle() > executionCycle) {
            continue;
        }

		INSTRUCTIONS opcode = instruction->getOpCode();
        unsigned int executionCyclesLeft =
                instruction->getExecuteCyclesLeft();

        // If rsEntry is not ready, cannot schedule.
        if (executionCyclesLeft == 0) {
            continue;
        }

        if (opcode == SW || opcode == LW) {

            //Memory address will be calculated in the first cycle
            // Enforcing in order address computation.
            // While iterating RS, find out one with min cycles,
            // later execute the address calc for the first one.
            if (executionCyclesLeft == 2 &&     // if not started executing and
                 rsEntry->getQj() == RSEntry::DEFAULT_Q &&
                    (!minLoadStoreEntry ||      // if first LW or SW, then its min
                        // else if scheduled before, then that is min.
                        minLoadStoreEntry->getInstruction()->getExecutionCycle()
                        < instruction->getExecutionCycle())) {

                    minLoadStoreEntry = rsEntry;

            // If one cycle left, do memory access and
            // write to cdb if LW or
            // mark ready in ROB if SW and register to be written is ready.
            } else if (executionCyclesLeft == 1) {
                if (opcode == SW) {
                    // TODO::Verify that we have to wait for both Qj and Qk to be
                    // zero for SW instruction.
                    // If so, this check not needed
                    if (rsEntry->getQk() == RSEntry::DEFAULT_Q) {
                        instruction->decrementExecuteCyclesLeft();
                        instruction->getROBSlot()->makeReady();
                        instruction->setExecutionCycle(executionCycle + 1);
                        instruction->getROBSlot()->setValue(rsEntry->getVk());
                        instruction->getROBSlot()->setDestination(rsEntry->getAddress());
                        cout << "	Executing Final write of SW Inst: " << instruction->instructionString()<<endl;
                    }
                } else {
                    int earlyStorePresent = false;
                    for (vector<RSEntry*>::iterator itTemp = reservations.begin(); itTemp != reservations.end(); ++itTemp) {
                        RSEntry* rsEntryTemp = *itTemp;
                        Instruction* instructionTemp =
                        		rsEntryTemp->getInstruction();
                        INSTRUCTIONS opcodeTemp = instructionTemp->getOpCode();
                        // If this RS inst is SW and is scheduled before.
                        // say early store present.
                        // TODO:: Not very correct I guess.
                        // Should be ROB that decides
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
                        instruction->setExecutionCycle(executionCycle + 1);
                        cout << "	Executing LW Inst final phase: " << instruction->instructionString()<<endl;
                        instruction->decrementExecuteCyclesLeft();
                    }
                }
            }
        } else if (rsEntry->isReady()) {
            if (instruction->isBranchInst() || opcode == J) {
                bool outcome = instruction->outcome(rsEntry->getVj(),
                        rsEntry->getVk());

                cout << "	Executing Branch Or Jump Inst: " << instruction->instructionString()<<endl;
                unsigned int newAddress = instruction->getDestination();

                //Update BTB
                btb->updateOrAdd(instruction->getAddress(),
                        (outcome ? BTBEntry::TAKEN : BTBEntry::NOT_TAKEN));

                //Update ROB with the destination and branch outcome
                instruction->getROBSlot()->setDestination(newAddress);

                // Update instruction about the outcome.
                cout << instruction->getOutCome() << endl;
                if(outcome != instruction->getOutCome()) {
                    instruction->setFlush();
                }

                instruction->setOutCome(outcome);
                //Mark the instruction ready
                instruction->getROBSlot()->makeReady();
                instruction->setExecutionCycle(executionCycle + 1);
                instruction->decrementExecuteCyclesLeft();
                aluUsed++;
              //  if (outcome) {
              //      nextPC = newAddress;
              //  }
            } else {
                //Write the result to CDB
                cout << "	Executing ALU Inst: " << instruction->instructionString()<<endl;
                cdb->set(instruction->getROBSlot()->getIndex(),
                        instruction->execute(rsEntry->getVj(),
                                rsEntry->getVk()));
                instruction->setExecutionCycle(executionCycle + 1);
                aluUsed++;
                instruction->decrementExecuteCyclesLeft();
            }
        }   // if (rsEntry->ready())
	}       // for loop

	// Address should be calculated for The LW\SW entry at the head of the queue.
	if (minLoadStoreEntry) {

		minLoadStoreEntry->setAddress(
				minLoadStoreEntry->getInstruction()->execute(
						minLoadStoreEntry->getVj(),
						minLoadStoreEntry->getVk()));

		minLoadStoreEntry->getInstruction()->decrementExecuteCyclesLeft();
		cout << "	Executing Add translation of LW or SW Inst: " << minLoadStoreEntry->getInstruction()->instructionString()<<endl;


        Instruction* minLWSWInst = minLoadStoreEntry->getInstruction();
        if (minLWSWInst->getOpCode() == SW &&
                minLoadStoreEntry->getQk() == RSEntry::DEFAULT_Q) {

                cout << "	Vk available, executing Final write of SW Inst: " << minLWSWInst->instructionString()<<endl;
                minLWSWInst->decrementExecuteCyclesLeft();
                minLWSWInst->getROBSlot()->makeReady();
                minLWSWInst->setExecutionCycle(executionCycle + 1);
                minLWSWInst->getROBSlot()->setValue(minLoadStoreEntry->getVk());
                minLWSWInst->getROBSlot()->setDestination(minLoadStoreEntry->getAddress());
        }
	}

}

void Executor::writeResultStage() {
    // When result available in CDB
    //      update ROB
    //      make ROB instruction ready to commit in next cycle.
	rob->updateFromCDB();

    //  update RS.
	resStation->updateFromCDB();

    // Flush CDB at the end of this cycle
	cdb->flush();
}

void Executor::commitStage() {
    // If rob empty, nothing to be done
	if (rob->isEmpty()) {
		return;
	}

	ROBSlot* slot = rob->peekTop();
	Instruction* inst = slot->getInstruction();

    // Check if commit is scheduled in this cycle
    // If not scheduled in this cycle or not ready, do nothing
	if (!slot->isReady() || inst->getExecutionCycle() > executionCycle) {
		return;
	}

    // Commit is in order. One instruction from top of the queue.
	rob->dequeueInstruction();

	unsigned int destination = slot->getDestination();
	INSTRUCTIONS opCode = inst->getOpCode();

    if (opCode == BREAK) {
       // If BREAK, this is the last cycle.
       cout << " 	Commiting BREAK " << inst->instructionString() << endl;
       done = true;
       return;
    // Branching:
    } else if (opCode == J || inst->isBranchInst()) {
		// check the ROB's next instruction's address
		// is same as destination
		cout << " 	Commiting J OR BRANCH " << inst->instructionString() << endl;

		//  if not same,
		if (inst->shallFlush()) {
			// then flush the whole system
			// (ROB, RS, IQ, Register Status)
			cout << " 	BRANCH not taken" << endl;
			flush();
			nextPC = slot->getDestination();
			return;
		}

	} else if (opCode == SW) {
		// update destination memory with value
		writeToMemory(destination, new Data(destination, slot->getValue()));
		cout << " 	Commiting SW " << inst->instructionString() << "   " << destination << "==>" << getMemoryData(destination)->getValue()<< endl;

    } else if (opCode != NOP) {
    	cout << " 	Commiting ALU and LW inst " << inst->instructionString() << endl;
        // For ALU  and load instruction,
        // register is updated with the result
        registers->set(destination, slot->getValue());
        regStatus->reset(destination);
    }

    // finally vacate the reservation station.
	int RSId = inst->getRSId();
    if (RSId != -1) {
        resStation->remove(RSId);
    }
}

void Executor::run() {

    logFile.open(getLogFileName(), ofstream::out);
	while (!done) {
		executionCycle++;

		cout << "Execution Cycle: " << executionCycle<<endl;
		instFetchStage();
		decodeStage();
        if (startCycle != -1 && endCycle != -1
                && executionCycle >= startCycle
                && executionCycle <= endCycle) {

            logFile << "Cycle <" << executionCycle << ">:"<<endl;
            dumpLog();

        }
		executeStage();
		writeResultStage();
		commitStage();
	}

    logFile << "Final Cycle <" << executionCycle << ">:"<<endl;
    dumpLog();
    logFile.close();
}

void Executor::flush() {
	rob->flush();
	resStation->flush();
	regStatus->flush();
	cdb->flush();
    instructionQueueFlush();
}


string Executor::instructionQueueDump() {
    stringstream ss;
    ss << "IQ:";
    for (deque<RawInstruction*>::iterator it = instructionQueue.begin();
            it != instructionQueue.end(); it++) {
        ss << endl << "[";
        Instruction* inst = InstructionBuilder::build((*it)->getAddress(),
                (*it)->getBitString());
        ss << inst->instructionString();
        ss << "]";
        if (inst->getOpCode() != NOP) {
            ss << " ";
        }
    }
    return ss.str();
}

string Executor::memoryDump() {
   int startAddress = 716;
   stringstream ss;
   ss << "Data Segment:"<<endl;
   ss << startAddress << ":";
   for (int index = getIndexFromAddress(startAddress);
               index < memory.size(); index++) {
        ss << "\t" << memory[index]->getValue();
   }
    return ss.str();
}


void Executor::dumpLog() {
    logFile << instructionQueueDump()<<endl;
    logFile << resStation->resStationDump()<<endl;
    logFile << rob->robDump()<<endl;
    logFile << btb->btbDump()<<endl;
    logFile << registers->registersDump()<<endl;
    logFile << memoryDump()<<endl;
}
