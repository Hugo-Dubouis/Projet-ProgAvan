#OBJS specifies which files to compile as part of the project
OBJS = main.cpp game.cpp game.hpp snake.cpp snake.hpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
# Windows
ifeq ($(OS),Windows_NT)
	LINKER_FLAGS = -lmingw32 -lSDL2 -lSDL2main -lmingw32 -lSDL2main -lSDL2
# Linux
else
	LINKER_FLAGS = -lSDL2 
endif

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = snake.exe

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)