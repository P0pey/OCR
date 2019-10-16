#Makefile

CC=gcc -fsanitize=address -rdynamic

CPPFLAGS =  `pkg-config gtk+-3.0 --cflags sdl` -MMD
CFLAGS = -Wall -Wextra -std=c99 -g3
LDFLAGS =
LDLIBS= `pkg-config gtk+-3.0 --libs sdl` -lSDL_image -lm

SRC = main.c segmentation/sdl_operations.c segmentation/segmentation.c interface/gtk.c  segmentation/rlsa.c NeuralNetwork/ocr.c NeuralNetwork/train.c NeuralNetwork/layer.c NeuralNetwork/neural_network.c
OBJ= $(SRC:.c=.o)
DEP= $(SRC:.c=.d)

all: main

main: $(OBJ)

clean:
		$(RM) $(OBJ) $(DEP) main

