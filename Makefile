APP = compiler
CXX = g++
FLAGS = -std=c++17 -Wall -I ./include
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.cpp)
BIN_DIR = build

EXT = calc
TSTDIR = tests
TESTS = $(wildcard $(TSTDIR)/*.$(EXT))
TESTS := $(TESTS:%.$(EXT)=%)

OBJ := $(SRC:.cpp=.o)
OBJ := $(OBJ:$(SRC_DIR)/%=%)
OBJ := $(addprefix $(BIN_DIR)/,$(OBJ))

.PHONY: all clean test test-clean

all: $(APP) 

$(BIN_DIR): 
	mkdir -p $(BIN_DIR)
$(APP): $(BIN_DIR) $(OBJ) 
	$(CXX) $(FLAGS) $(OBJ) -o $@
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp 
	$(CXX) $(FLAGS) -c $< -o $@
clean: test-clean
	rm -rf $(BIN_DIR) $(APP)
	rm -f output.o output.s a.out
test-clean:
	rm -f $(TSTDIR)/*.asm $(TSTDIR)/*.out $(TSTDIR)/*.o $(TESTS)

test: $(TESTS) 
	@for item in $(TESTS); do \
		./$$item > $$item.out; \
		if diff -u ./$$item.out ./$$item.tst; then echo "Test $$item is OK"; else echo "Warning!!! Test $$item is failed!!!"; fi \
	done

$(TESTS): % : %.$(EXT) %.tst $(APP)
	./$(APP) $< $@.asm
	nasm -f elf $@.asm
	ld -melf_i386 $@.o -o $@
	./$@ > $@.out

#ld -lc -melf_i386 -dynamic-linker /lib/ld-linux.so.2 $< -o $@
