CC = gcc
CFLAGS = -Wall -Iinclude `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

SRC = $(wildcard src/*.c)
OBJ = $(SRC:*.c=obj/*.o)
TARGET = factorio

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

