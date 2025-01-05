TARGET = nish-c
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

CGLAGS = -std=c99 -Wall -g -Werror -Wextra


default: clean $(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)

$(TARGET): $(OBJ)
	$(CC) $(CGLAGS) -o $@ $^


*.o: *.c
	$(CC) $(CGLAGS) -c $^
