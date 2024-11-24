CFLAGS=-std=gnu99
ROOT_DIR=C:/Users/philp/Projects/ccheckers2
INCLUDE_PATHS =  -IC:\dev\SDL2-2.30.9\include\SDL2 -LC:\dev\SDL2-2.30.9\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2

build:
	gcc $(CFLAGS) $(ROOT_DIR)/src/ccheckers2.c $(INCLUDE_PATHS) -Wall -o ./bin/ccheckers2 