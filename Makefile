CC = gcc
CFLAGS = -Iinclude
LDFLAGS = -Llib
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

SRC = main.c enemies.c player.c
EXEC = main

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -g $< -o $@ $(LIBS)

run: $(EXEC)
	./$(EXEC)

clean:
	del $(EXEC).exe
