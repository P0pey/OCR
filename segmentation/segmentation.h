# ifndef SEGMENTATION_H_
# define SEGMENTATION_H_

#include <SDL/SDL.h>

void cutlines(SDL_Surface *img);
SDL_Surface* displayline(SDL_Surface *img, int line);
int linenumber(SDL_Surface *img);
void cutchar(SDL_Surface *img);
SDL_Surface* displaychar(SDL_Surface *img, int number, int max);
int charnumber(SDL_Surface *img);
# endif
