#ifndef _RLSA_H_
#define _RLSA_H_

#include "sdl_operations.h"

void rlsa(SDL_Surface *img, int n);
int *vert_rlsa(SDL_Surface *img, int n);
int *hor_rlsa(SDL_Surface *img, int n);

#endif
