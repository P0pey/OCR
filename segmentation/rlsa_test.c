#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "sdl_operations.h"
#include "rlsa.h"

int main(void)
{
    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    init_sdl();

    image_surface = load_image("journal.jpg");
    screen_surface = display_image(image_surface);

    wait_for_keypressed();

    grayscale(image_surface);
    update_surface(screen_surface, image_surface);

    wait_for_keypressed();

    binarize(image_surface);
    update_surface(screen_surface, image_surface);

    wait_for_keypressed();

    rlsa(image_surface, 950);
    update_surface(screen_surface, image_surface);

    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);

    return 0;
}
