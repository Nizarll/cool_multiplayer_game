# Makefile for your raylib project

# Compiler settings
CC := gcc
C_STANDARD := --std=c2x
CFLAGS := -Wall -I"C://raylib//raylib//src" $(C_STANDARD)
LDFLAGS := -L"C://raylib//raylib//src"
LIBS := -lraylib -lopengl32 -lgdi32 -lwinmm

# Source and object directories
SRC_DIR := src
OBJ_DIR := obj

# List all source files
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)

# Generate corresponding object file names
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Target executable
TARGET := output

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(OBJ_FILES) $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

