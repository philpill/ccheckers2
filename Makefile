CFLAGS = -std=gnu99
ROOT_DIR = C:\Users\philp\Projects\ccheckers2
SDL_DIR = C:\dev\SDL2-2.30.9
OBJS = /*.c 
INCLUDE_PATHS = -I$(SDL_DIR)\include\SDL2 -I$(ROOT_DIR)\include
LIBRARY_PATHS = -L$(SDL_DIR)\lib
COMPILER_FLAGS = -Wl,-subsystem,windows -Wall
COMPILER_FLAGS2 = -Wall
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2
OBJ_NAME = ccheckers2

all: build run

run:
	$(ROOT_DIR)\bin\ccheckers2.exe

build:
	gcc $(CFLAGS) src\$(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS2) $(LINKER_FLAGS) -o $(ROOT_DIR)\bin\$(OBJ_NAME).exe



