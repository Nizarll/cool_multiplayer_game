C_STANDARD := --std=c2x
RAYLIB_PATH := "C://raylib//raylib//src"
CC := gcc
CFLAGS :=  -I$(RAYLIB_PATH) $(C_STANDARD)
LDFLAGS := -L$(RAYLIB_PATH)
LIBS := -lraylib -lopengl32 -lgdi32 -lwinmm
SRC := $(wildcard src/*.c) main.c
OBJ := $(SRC:.c=.o)
# Output binary
TARGET := my_game

.PHONY: clean

all: $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)


.INTERMEDIATE: $(OBJ)
