GTEST_DIR = ${HOME}/gtest/
CC = g++
INCLUDES = -I${GTEST_DIR}/include
DEP_FILES = src/commons.cpp \
		 src/simulator/SimulatorBuilder.cpp src/simulator/Executor.cpp \
		src/instructions/InstructionBuilder.cpp src/instructions/I/IInstruction.cpp \
		src/instructions/R/RInstruction.cpp src/instructions/J/JInstruction.cpp src/instructions/OpcodeMap.cpp	\
		src/pipeline/ROB.cpp src/pipeline/ReservationStation.cpp src/pipeline/BTB.cpp src/pipeline/CDB.cpp
SOURCE_FILES=src/MIPSSim.cpp $(DEP_FILES)
OBJECT_FILES = $(SOURCE_FILES:.cpp=.o)
EXECUTABLE = target/MIPSSim

DEPS = $(OBJECT_FILES:.o=.d)
all: $(SOURCE_FILES) $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE_FILES)
	${CC} -g -o0 $(SOURCE_FILES) -o $@

.cpp.o:
	${CC} -c -g -o0 $< -o $@

gtest:
	${CC} -g -o0 -I${GTEST_DIR}/include -L${GTEST_DIR} -pthread test/test.cpp $(DEP_FILES) -lgtest -o target/test.out

clean:
	rm -rf ./*.o target/* src/*.o src/instructions/*.o src/simulator/*.o target/*.out src/pipeline/*.out src/instructions/I/*.out src/instructions/R/*.out src/instructions/J/*.out
