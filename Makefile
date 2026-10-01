# Source files
CC = gcc

SRC = src/*.c
HEADER  = include/*.h
OUT = usrlang.exe

CFLAGS = -Wall -Wextra -std=c99   
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

# Default arguments (empty if not specified)
ARGS = 

all: build run

build:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LIBS)

# Added $(ARGS) to the execution commands
run:
	./$(OUT) $(ARGS)

check:
	$(CC) -g $(SRC) -o $(OUT) $(LIBS)
	gdb ./$(OUT) 

.PHONY : clean format all build run check
clean:
	rm -f *.o *.exe FrontEnd/*.o

format:
	clang-format -i $(SRC) $(HEADER)

