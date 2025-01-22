TARGET = nish
CC = cc
SRC = $(wildcard src/*.c)
OBJ_DIR = obj
OBJ = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRC))

CFLAGS = -std=c11 -Wall -g -Werror -Wextra -D_POSIX_C_SOURCE=200809L $(shell pkg-config --cflags lua)
LDFLAGS = $(shell pkg-config --libs lua)

default: clean $(TARGET)

clean:
	rm -rf $(TARGET) $(OBJ_DIR)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) 

$(OBJ_DIR)/%.o: src/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<
