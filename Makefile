chip8: main.c chip8.c
	gcc -g `pkg-config --cflags --libs sdl2` main.c chip8.c -o chip8

clean:
	rm chip8
