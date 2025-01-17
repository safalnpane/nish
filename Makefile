TARGET = nish
SRC = $(wildcard src/*.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

CGLAGS = -std=c11 -Wall -g -Werror -Wextra


default: clean $(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)

$(TARGET): $(OBJ)
	$(CC) $(CGLAGS) -o $@ $^


*.o: *.c
	$(CC) $(CGLAGS) -c $^
