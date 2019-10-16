#include "rlsa.h"


void rlsa(SDL_Surface *img, int n)
{
    int *vert = vert_rlsa(img, n);
    int *hor = hor_rlsa(img, n);
    for(int i = 0; i < img->w; i++)
    {
        for(int j = 0; j < img->h; j++)
        {
            Uint8 rgb;
            if(vert[i + j * img->w] == 0 && hor[i + j * img->w] == 0)
                rgb = 0;
            else
                rgb = 255;
            //rgb = vert[i + j * img->w];
            Uint32 pixel = SDL_MapRGB(img->format, rgb, rgb, rgb);
            put_pixel(img, i, j, pixel); 
        }
    }
    free(hor);
    free(vert);
}

int *vert_rlsa(SDL_Surface *img, int n)
{
    Uint32 pixel;
    Uint8 r, g , b;

    int *vert;
    vert = malloc(sizeof(int) * img->w * img->h);
    for(int i = 0; i < img->w; i++)
    {
        int count = 0;
        for(int j = 0; j < img->h; j++)
        {
            pixel = get_pixel(img, i, j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            vert[i + j * img->w] = r;
            if(r == 0)
            {
                if(count <= n)
                    for(int k = j; k >= j - count; k--)
                        vert[i + k * img->w] = 0;
                count = 0;
            }
            else
                count++;
        }
    }
    return vert;
}

int *hor_rlsa(SDL_Surface *img, int n)
{
    Uint32 pixel;
    Uint8 r, g, b;

    int *hor;
    hor = malloc(sizeof(int) * img->w * img->h);
    for(int j = 0; j < img->h; j++)
    {
        int count = 0;
        for(int i = 0; i < img->w; i++)
        {
            pixel = get_pixel(img, i, j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            hor[i + j * img->w] = r;
            if(r == 0)
            {
                if(count <= n)
                    for(int k = i; k >= i - count; k--)
                        hor[k + j * img->w] = 0;
                count = 0;
            }
            else
                count++;
        }
    }
    return hor;
}
