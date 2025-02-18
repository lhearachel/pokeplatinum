CFLAGS += -MMD -std=c99 -Wall -Wextra -g3

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

TARGET = makerom

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

all: $(TARGET)

clean:
	$(RM) $(OBJ) $(DEP) $(TARGET) compile_commands.json

-include $(DEP)
