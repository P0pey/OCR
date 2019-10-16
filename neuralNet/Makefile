# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -std=c99 -O0
LDFLAGS=
LDLIBS= -lm `pkg-config --libs sdl` -lSDL_image

all: xor ocr

xor : train.o layer.o neural_network.o ../segmentation/sdl_operations.o
ocr : train.o layer.o neural_network.o ../segmentation/sdl_operations.o
xor.o : train.h layer.h neural_network.h ../segmentation/sdl_operations.h
ocr.o : train.h layer.h neural_network.h ../segmentation/sdl_operations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} xor
	${RM} ocr


# END
