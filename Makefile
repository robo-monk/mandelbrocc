# Define compiler and flags
CC = clang
CFLAGS = -Wall -Wextra $(shell sdl2-config --cflags) -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

LIBS = $(shell sdl2-config --libs) -lSDL2_ttf

# Define source, object, and binary directories
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = $(OBJ_DIR)
# BIN_DIR = bin

# Automatically list all the C source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Generate corresponding object file names
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Define the final binary name
BIN = $(BIN_DIR)/mandel

# Default target
all: $(BIN)
	@echo "Building completed."

# Link object files to create the final binary
$(BIN): $(OBJS)
	$(CC) $^ -o $@ $(LIBS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create the build directory
$(OBJ_DIR):
	mkdir -p $@

# Clean up the build directory
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJ_DIR)

# Run the program
run: $(BIN)
	@echo "\n\n"
	time ./$<

# Phony targets
.PHONY: all clean run
