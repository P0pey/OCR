# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image

all: segmentation grayscale rlsa_test

segmentation: sdl_operations.o segmentation.o
segmentation.o: sdl_operations.h 
grayscale: sdl_operations.o grayscale.o
grayscale.o: sdl_operations.h
rlsa_test: sdl_operations.o rlsa.o
rlsa.o: sdl_operations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} segmentation
	${RM} grayscale
	${RM} rlsa_test
# END
