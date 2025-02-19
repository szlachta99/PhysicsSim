# Compiler and flags
CC = gcc
CDEBUGFLAGS = -fdiagnostics-color=always -g
CFLAGS = -Wall -Iinclude -std=c17 $(shell pkg-config --cflags sdl3)
LDFLAGS = $(shell pkg-config --libs sdl3) -lm

# Directories
SRC_DIR = src
BUILD_DIR = build
EXECUTABLE = simulation

# Find all source files in the src directory
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Default target
all: $(EXECUTABLE)

# Link the executable
$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

debug: CFLAGS += $(CDEBUGFLAGS)
debug: all

run: $(EXECUTABLE)
	./$(EXECUTABLE)

# Clean the build directory and executable
clean:
	rm -rf $(BUILD_DIR) $(EXECUTABLE)

.PHONY: all clean run
