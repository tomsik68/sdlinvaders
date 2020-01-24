CC:=clang
CFLAGS:= -std=c11 -Wall -Wextra -pedantic $(shell sdl2-config --cflags) -g
LDFLAGS:= $(shell sdl2-config --libs) -lSDL2_image

all: main

main: main.o game.o spritesheet.o

%.o: %.c %.h

clean:
	rm -f *.o main
