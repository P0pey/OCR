#include "sdl_operations.h"
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

static inline
Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

void update_surface(SDL_Surface* screen, SDL_Surface* image)
{
    if (SDL_BlitSurface(image, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}

SDL_Surface* square_char(SDL_Surface* image){
    if(image->w == image->h)
        return image;
    else{
        Uint32 pixel;
        pixel = SDL_MapRGB(image->format, 255, 255, 255);
        SDL_Surface* temp = NULL;
        if(image->w > image->h){
            temp = SDL_CreateRGBSurface(0, image->w, image->w, 32, 0, 0, 0, 0);
            for(int i = 0; i < temp->w; i++){
                for(int j = 0; j < temp->w; j++)
                    put_pixel(temp, i ,j, pixel);
            }
            int average = (image->w - image->h) / 2;
            for(int i = 0; i < image->w; i++){
                for(int j = 0; j < image->h; j++)
                    put_pixel(temp, i, j + average, get_pixel(image, i, j));
            }
        }
        else{
            temp = SDL_CreateRGBSurface(0, image->h, image->h, 32, 0, 0, 0, 0);
            for(int i = 0; i < temp->h; i++){
                for(int j = 0; j < temp->h; j++)
                    put_pixel(temp, i, j, pixel);
            }
            int average = (image->h - image->w) / 2;
            for(int i = 0; i < image->w; i++){
                for(int j = 0; j < image->h; j++)
                    put_pixel(temp, i + average, j, get_pixel(image, i, j));
            }
        }
        return temp;
    }
}

SDL_Surface* resize_char(SDL_Surface* image, int width, int height){
    SDL_Surface *temp = NULL;
    temp = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

    if(temp == NULL)
        errx(1, "Failed to create the resize image");

    if(image->w < 20)
        image = square_char(image);
    
    /*int ratio = image->h / width;
    for(int i = 0; i < width; i++){
        for(int j = 0; j < height; j++){
            Uint32 pxl = 0;
            for(int x = 0; x < ratio; x++){
                for(int y = 0; y < ratio; y++)
                    pxl += get_pixel(image, x + i*ratio, y + j*ratio);
            }
            Uint32 average = pxl / (ratio * ratio);
            put_pixel(temp, i, j, average);
        }
    }
    grayscale(temp);
    binarize(temp);
    return temp;*/
    image = Resize(image, width, height);
    grayscale(image);
    binarize_char(image);
    return image;
}

// More pricise (less black pixels)
void binarize_char(SDL_Surface *img){
    Uint32 pxl;
    Uint8 r, g, b;
    int W = img->w, H = img->h;
    for(int i = 0; i < W; i++){
        for(int j = 0; j < H; j++){
            pxl = get_pixel(img, i, j);
            SDL_GetRGB(pxl, img->format, &r, &g, &b);
            r = r >= 10 ? 255 : 0;
            g = g >= 10 ? 255 : 0;
            b = b >= 10 ? 255 : 0;
            pxl = SDL_MapRGB(img->format, r, g, b);
            put_pixel(img, i, j, pxl);
        }
    }
}

void binarize(SDL_Surface *img)
{
    Uint32 pixel;
    Uint8 r, g , b;
    int  w = img -> w, h = img -> h;
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            pixel = get_pixel(img,i,j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            r = r >= 127 ? 255 : 0;
            g = g >= 127 ? 255 : 0;
            b = b >= 127 ? 255 : 0;
            pixel = SDL_MapRGB(img->format, r, g, b);
            put_pixel(img, i, j, pixel);
        }
    }
}

void grayscale(SDL_Surface *img)
{
    int width = img->w;
    int height = img->h;

    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            Uint32 pixel = get_pixel(img, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            int average = 0.3*r + 0.59*g + 0.11*b;
            r = average, g = average, b = average;
            Uint32 pixel1 = SDL_MapRGB(img->format, r, g, b);
            put_pixel(img, x, y, pixel1);
        }
    }
}
SDL_Surface* Resize(SDL_Surface *img, int width, int height)
{
  SDL_Surface *dest = SDL_CreateRGBSurface(SDL_HWSURFACE,
                        width,
                        height,
                        img->format->BitsPerPixel,0,0,0,0);
  SDL_SoftStretch(img, NULL, dest, NULL);
  return dest;
}

