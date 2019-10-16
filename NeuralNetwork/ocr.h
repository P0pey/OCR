#ifndef _OCR_H_
#define _OCR_H_

#include <stdlib.h>
#include <err.h>
#include "neural_network.h"
#include "train.h"

void print_char(SDL_Surface *img);
void ocr_train(size_t n, int print, int save);
char ocr(SDL_Surface *img);

#endif
