RAYLIB_PATH := "C://raylib//raylib//src"
CC := gcc
CFLAGS := -Wall -Wextra -I$(RAYLIB_PATH)
LDFLAGS := -L$(RAYLIB_PATH) -lraylib
SRC := $(wildcard src/*.c) main.c
OBJ := $(SRC:.c=.o)
# Output binary
TARGET := my_game

.PHONY: clean

all: $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


.INTERMEDIATE: $(OBJECTS)
