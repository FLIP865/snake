CC = gcc
SRC = main.c
BIN = snake

all: $(BIN)
	@echo "Assembly complete, program launch..."
	./$(BIN)


$(BIN): $(SRC)
	@echo "Compiling prigram..."
	@$(CC) $(SRC) -o $(BIN) -lncurses -O2 -g

clean:
	rm -rf $(BIN)
	@echo "Cleaning coplite"

.PHONY: all clean
