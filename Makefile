SRC = process.c malloc_image.c

OBJ = process.o malloc_image.o

INCLUDES = -I.

LFLAGS = -L. -lm

CFLAGS = -ansi -Wall -pedantic -std=c99 -O -g $(INCLUDES)

CC = gcc

EXEC = process

$(EXEC): $(OBJ)
	$(CC) -o $(EXEC) $(OBJ) $(LFLAGS)

all: $(EXEC)

clean:
	rm -f *.o $(EXEC) core
