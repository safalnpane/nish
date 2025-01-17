TARGET = nish
SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

CFLAGS = -std=c11 -Wall -g -Werror -Wextra $(shell pkg-config --cflags lua)
LDFLAGS = $(shell pkg-config --libs lua)


default: clean $(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^


*.o: *.c
	$(CC) $(CFLAGS) -c $^
