CC:=clang
CFLAGS:= -std=c11 -Wall -Wextra -pedantic $(shell sdl2-config --cflags) -g
LDFLAGS:= $(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf -lSDL2_mixer
OBJECTS := main.o game.o spritesheet.o text.o ui.o sound.o

all: main

main: $(OBJECTS)

%.o: %.c %.h

clean:
	rm -f *.o main
