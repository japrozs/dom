CC = clang 
CFLAGS = -Wall -Wextra -pedantic -I include -g
SRC = $(wildcard src/*.c)
HEAD = $(wildcard include/*.h)
OBJ = $(SRC:.c=.o)
EXEC = out/dom

all: clean $(OBJ) $(EXEC) $(HEAD)

$(EXEC): $(OBJ)
	$(CC)  $^ -o $@
	rm -rf src/*.o

%.o: %.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o src/*.o $(EXEC)
