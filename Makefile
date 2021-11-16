CPP = src/*.cpp
CC = g++
MINGW = x86_64-w64-mingw32-g++
WINFLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lopengl32 -lglew32 -o
FLAGS = -Wall -lGLEW -lGL -lSDL2 -lSDL2_image -lSDL2_ttf -o
NAME = test

all:
	$(CC) $(CPP) $(FLAGS) $(NAME)

run:
	$(CC) $(CPP) $(FLAGS) $(NAME)
	./$(NAME)

win:
	$(MINGW) $(CPP) $(WINFLAGS) windows/$(NAME).exe
