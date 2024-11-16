# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11 -I../include

# Directories
SRC_DIR = ../src
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SOURCES = $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Output executable
TARGET = $(BIN_DIR)/GenerateEncoder

# Default target to build
all: $(TARGET)

# Link the object files to create the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

# Compile the C++ source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)/$(dir $*)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up the build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets (to avoid conflict with file names)
.PHONY: all clean
