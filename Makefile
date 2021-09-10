CPP = src/*.cpp
CC = g++
FLAGS = -Wall -lGLEW -lGL -lSDL2 -lSDL2_image -lSDL2_ttf -o
NAME = test

all:
	$(CC) $(CPP) $(FLAGS) $(NAME)

run:
	$(CC) $(CPP) $(FLAGS) $(NAME)
	./$(NAME)
