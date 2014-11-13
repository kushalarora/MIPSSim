GTEST_DIR = ${HOME}/gtest/
CC = g++
INCLUDES = -I${GTEST_DIR}/include
SOURCE_FILES=src/MIPSSim.cpp src/commons.cpp \
		src/simulator/DisAssembler.cpp src/simulator/Executor.cpp src/simulator/SimulatorBuilder.cpp \
		src/instructions/InstructionBuilder.cpp src/instructions/I/IInstruction.cpp \
		src/instructions/R/RInstruction.cpp src/instructions/J/JInstruction.cpp src/instructions/OpcodeMap.cpp
OBJECT_FILES = $(SOURCE_FILES:.cpp=.o)
EXECUTABLE = target/MIPSSim

DEPS = $(OBJECT_FILES:.o=.d)
all: $(SOURCE_FILES) $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE_FILES)
	${CC} -g -o0 $(SOURCE_FILES) -o $@

.cpp.o:
	${CC} -c -g -o0 $< -o $@

test:
	${CC} -I${GTEST_DIR}/include -L${GTEST_DIR} -pthread test/test.cpp -lgtest  -o target/test.out

clean:
	rm -rf ./*.o target/* src/*.o src/instructions/*.o src/simulator/*.o
