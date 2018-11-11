APP = main
CXX = g++
FLAGS = -std=c++17 -Wall -I ./include
SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.cpp)

BIN_DIR = build

OBJ := $(SRC:.cpp=.o)
OBJ := $(OBJ:$(SRC_DIR)/%=%)
OBJ := $(addprefix $(BIN_DIR)/,$(OBJ))

.PHONY: all clean

all: $(BIN_DIR) $(APP)

$(BIN_DIR): 
	mkdir -p $(BIN_DIR)
$(APP): $(OBJ)
	$(CXX) $(FLAGS) $(OBJ) -o $@
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp 
#$(OBJ): $(SRC) 
	$(CXX) $(FLAGS) -c $< -o $@
clean:
	rm -rf $(BIN_DIR) $(APP)
