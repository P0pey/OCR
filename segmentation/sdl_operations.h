#ifndef SDL_OPERATIONS_H
#define SDL_OPERATIONS_H

#include <stdlib.h>
#include <SDL/SDL.h>

void init_sdl();
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
void wait_for_keypressed();
Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y);
void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);
void update_surface(SDL_Surface* screen, SDL_Surface* image);
SDL_Surface* square_char(SDL_Surface* image);
SDL_Surface* resize_char(SDL_Surface* image, int width, int height);
void binarize_char(SDL_Surface *img);
void binarize(SDL_Surface *img);
void grayscale(SDL_Surface *img);
SDL_Surface* Resize(SDL_Surface *img, int width, int height);
#endif
