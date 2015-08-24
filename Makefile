SRC = process.c malloc_image.c

OBJ = process.o malloc_image.o

INCLUDES = -I.

LFLAGS = -L. -lm

CFLAGS = -ansi -Wall -pedantic -std=c99 -g $(INCLUDES)

OPTIMISE = -O

CC = gcc

EXEC = process

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(LFLAGS) $(CFLAGS)

all: $(EXEC)

clean:
	rm -f *.o *.exe $(EXEC) core
