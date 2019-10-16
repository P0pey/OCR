# include "segmentation.h"
# include <stdlib.h>
# include <err.h>
# include "SDL/SDL.h"
# include "SDL/SDL_image.h"
# include "sdl_operations.h"



void cutlines(SDL_Surface *img)
//cut the lines of the image with red lines
{
    Uint32 pixel;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    int isBlack = 0;
    int firstcut = 1;
    int width = img->w;
    int height = img->h;
    for(int i = 0; i < height ; i++)
    {
        isBlack = 0;
        for(int j = 0;j < width; j++)
        {
            pixel = get_pixel(img,j,i);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            if(r == 0 && g == 0 && b == 0)
            {
                isBlack = 1;
                break;
            }
        }
        //if there is a black character and no first cut before
        if(isBlack == 1  && firstcut == 1)
        {
            for(int k = 0; k < width; k++)
            {
                pixel = SDL_MapRGB(img->format, 255, 0, 0);
                put_pixel(img, k, i-1, pixel);
            }
            firstcut = 0;
        }
        //if there is a white line and a first cut before
        if(isBlack == 0 && firstcut == 0)
        {
            for(int k = 0; k < width; k++)
            {
                pixel = SDL_MapRGB(img->format, 255, 0, 0);
                put_pixel(img, k, i, pixel);
            }
            firstcut = 1;
        }
    }
}

SDL_Surface* displayline(SDL_Surface *img, int line)
//get the line you want(if the number you want is too high, give the last line)
{
    if(line <= 0)
    {
        errx(1,"line < 1"); 
    }
    Uint32 pixel;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    int firstcut = 0;
    int secondcut = -1;
    int width = img->w;
    int height = img->h;
    int c = 0;
    //get the line
    for(int i = 0; i < height; i++)
    {
        pixel = get_pixel(img,0,i);
        SDL_GetRGB(pixel, img->format, &r, &g, &b);
        if(r == 255 && g == 0 && b == 0)
        {
            firstcut = i;
            if(firstcut != secondcut)
            {
                for(int j = firstcut + 1; j < height; j++)
                {
                    pixel = get_pixel(img, 0, j);
                    SDL_GetRGB(pixel, img->format, &r, &g, &b);
                    if(r == 255 && g == 0 && b == 0)
                    {
                        secondcut = j;
                        c += 1;
                        break;
                    }
                }
            }
        }
        if(c == line)
        {
            break;
        }
    }
    //display of the line
    SDL_Surface* copy = NULL;
    copy = SDL_CreateRGBSurface(0,width,secondcut - firstcut, 32, 0, 0, 0, 0);
    if(copy == NULL)
    {
        errx(1,"Failed to create the image");
    }
    for(int i = 0; i < copy->w; i++)
    {
        for(int j = 0; j < copy->h; j++)
        {
            Uint32 pxl = get_pixel(img, i, firstcut + j);
            put_pixel(copy, i, j, pxl);
        }
    }
    return copy;
}

int linenumber(SDL_Surface *img)
{
    Uint32 pixel;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    int firstcut = 0;
    int secondcut = -1;
    int height = img->h;
    int c = 0;
    for(int i = 0; i < height; i++)
    {
        pixel = get_pixel(img,0,i);
        SDL_GetRGB(pixel, img->format, &r, &g, &b);
        if(r == 255 && g == 0 && b == 0)
        {
            firstcut = i;
            if(firstcut != secondcut)
            {
                for(int j = firstcut + 1; j < height; j++)
                {
                    pixel = get_pixel(img, 0, j);
                    SDL_GetRGB(pixel, img->format, &r, &g, &b);
                    if(r == 255 && g == 0 && b == 0)
                    {
                        secondcut = j;
                        c += 1;
                        break;
                    }
                }
            }
        }
    }
    return c;
}

void cutchar(SDL_Surface *img)
//cut the characters of the line with green columns
{
    Uint32 pixel;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    int isBlack = 0;
    int firstcut = 1;
    int width = img->w;
    int height = img->h;
    for(int i = 0; i < width ; i++)
    {
        isBlack = 0;
        for(int j = 0;j < height; j++)
        {
            pixel = get_pixel(img,i,j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            if(r == 0 && g == 0 && b == 0)
            {
                isBlack = 1;
                break;
            }
        }
        //if there is a black character and no first cut before
        if(isBlack == 1  && firstcut == 1)
        {
            for(int k = 0; k < height; k++)
            {
                pixel = SDL_MapRGB(img->format, 0, 255, 0);
                put_pixel(img, i-1, k, pixel);
            }
            firstcut = 0;
        }
        //if there is a white column and a first cut before
        else if(isBlack == 0 && firstcut == 0)
        {
            for(int k = 0; k < height; k++)
            {
                pixel = SDL_MapRGB(img->format, 0, 255, 0);
                put_pixel(img, i, k, pixel);
            }
            firstcut = 1;
        }
    }
}
/*
SDL_Surface* displaychar(SDL_Surface *img, int number)
{
    if(number <= 0)
    {
        errx(1,"number < 1");
    }
    Uint32 pixel;
    Uint8 r;
    Uint8 g;
    Uint8 b;
    int firstcut = 0;
    int secondcut = -1;
    int width = img->w;
    int height = img->h;
    int c = 0;
    //get the column
    for(int i = 0; i < width; i++)
    {
        pixel = get_pixel(img,i,0);
        SDL_GetRGB(pixel, img->format, &r, &g, &b);
        if(r == 0 && g == 255 && b == 0)
        {
            firstcut = i+1;
            pixel = get_pixel(img, firstcut, 0);
            SDL_GetRGB(pixel, img -> format, &r, &g, &b);
            while(r == 0 && g == 255 && b == 0)
            {
                firstcut++;
                pixel = get_pixel(img, firstcut, 0);
                SDL_GetRGB(pixel, img -> format, &r, &g, &b);
            }
            if(firstcut != secondcut + 2)
            {
                for(int j = firstcut+1; j < width; j++)
                {
                    pixel = get_pixel(img, j, 0);
                    SDL_GetRGB(pixel, img->format, &r, &g, &b);
                    if(r == 0 && g == 255 && b == 0)
                    {
                        secondcut = j-1;
                        c += 1;
                        break;
                    }
                }
            }
        }
        if(c == number)
        {
            break;
        }
    }
    //display of the char
    SDL_Surface* copy = NULL;
    copy = SDL_CreateRGBSurface(0,secondcut-firstcut,height, 32, 0, 0, 0, 0);
    if(copy == NULL)
    {
        errx(1,"Failed to create the image");
    }
    for(int i = 0; i < copy->w; i++)
    {
        for(int j = 0; j < copy->h; j++)
        {
            Uint32 pxl = get_pixel(img,firstcut + i, j);
            put_pixel(copy, i, j, pxl);
        }
    }
    copy = resize_char(copy, 28, 28);
    return copy;
}


int charnumber(SDL_Surface *img)
{
	int c = 0;
	Uint32 pixel;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	int firstcut = 0;
	int secondcut = -1;
	int width = img->w;
	//get the column
	for(int i = 0; i < width; i++)
	{
		pixel = get_pixel(img,i,0);
		SDL_GetRGB(pixel, img->format, &r, &g, &b);
		if(r == 0 && g == 255 && b == 0)
		{
			firstcut = i+1;
			pixel = get_pixel(img, firstcut, 0);
      		SDL_GetRGB(pixel, img -> format, &r, &g, &b);
      		while(r == 0 && g == 255 && b == 0)
      		{
        		firstcut++;
        		pixel = get_pixel(img, firstcut, 0);
        		SDL_GetRGB(pixel, img -> format, &r, &g, &b);
			}
			if(firstcut != secondcut + 2)
			{
				for(int j = firstcut + 1; j < width; j++)
				{
					pixel = get_pixel(img, j, 0);
					SDL_GetRGB(pixel, img->format, &r, &g, &b);
					if(r == 0 && g == 255 && b == 0)
					{
						secondcut = j-1;
						c += 1;
						break;
					}
				}
			}
		}
	}
	return c;
}*/

SDL_Surface *displaychar(SDL_Surface *img, int number, int max){
	Uint32 pxl;
	Uint8 r, g, b;
	
    // necessary a '\n' char (ref = 2)
	if(number == max){
		SDL_Surface *copy = NULL;
		copy = SDL_CreateRGBSurface(0, 28, 28, 32, 0, 0, 0, 0);
    	if(copy == NULL){
        	errx(1,"Failed to create the image");
    	}
		pxl = SDL_MapRGB(copy->format, 255, 255, 255);
    	for(int i = 0; i < copy->w; i++){
        	for(int j = 0; j < copy->h; j++){
       			put_pixel(copy, i, j, pxl);
        	}
    	}
		pxl = SDL_MapRGB(copy->format, 2, 2, 2);
		put_pixel(copy, 0, 0, pxl);
	}
	

	int c = 0, len = 0, firstcut = 1;
	for(int i = 0; i < img->w; i++){
		pxl = get_pixel(img, i, 0);
		SDL_GetRGB(pxl, img->format, &r, &g, &b);
		if(g == 255)
			c++;
		if(c == number){
			len = 1;
			i++;
			firstcut = i;
			pxl = get_pixel(img, i, 0);
			SDL_GetRGB(pxl, img->format, &r, &g, &b);
			while(g != 255){
				len++;
				i++;
				pxl = get_pixel(img, i, 0);
				SDL_GetRGB(pxl, img->format, &r, &g, &b);
			}
			break;
		}
	}

    // create a new image (cut char)
	SDL_Surface *copy = NULL;
	copy = SDL_CreateRGBSurface(0, len, img->h, 32, 0, 0, 0, 0);
    if(copy == NULL)
    {
        errx(1,"Failed to create the image");
    }
    int isBlack = 0;
    for(int i = 0; i < copy->w; i++)
    {
        for(int j = 0; j < copy->h; j++)
        {
            pxl = get_pixel(img,firstcut+i, j);
            SDL_GetRGB(pxl, img->format, &r, &g, &b);
            // check if it exist a black pixel
            if(r == 0 && g == 0 && b == 0)
                isBlack = 1;
            put_pixel(copy, i, j, pxl);
        }
    }

	/*if(len < 5){
        int cha = 0;
        for(int i = 0; i < copy->w; i++){
            for(int j = 0; j < copy->h; j++){
                pxl = get_pixel(copy, i, j);
                SDL_GetRGB(pxl, copy->format, &r, &g, &b);
                if(r == 0 && g == 0 && b == 0)
                    cha = 1;
            }
            if(cha == 1)
                break;
        }
        if(cha == 1)
            copy = resize_char(copy, 28, 28);
        else{
            pxl = SDL_MapRGB(copy->format, 3, 3, 3);
		    put_pixel(copy, 0, 0, pxl);
		}
        return copy;
	}
	else{
		int cha = 0;
		for(int i = 0; i < copy->w; i++){
			for(int j = 0; j < copy->h; j++){
				pxl = get_pixel(copy, i, j);
				SDL_GetRGB(pxl, copy->format, &r, &g, &b);
				if(r == 0 && g == 0 && b == 0)
					cha = 1;	
			}
			if(cha == 1)
				break;
		}
		if(cha == 1)
    		copy = resize_char(copy, 28, 28);
		else{
			pxl = SDL_MapRGB(copy->format, 1, 1, 1);
			put_pixel(copy, 0, 0, pxl);
		}
		return copy;
    }*/

    // it's a character so resize it for the OCR
    if(isBlack == 1)
        copy = resize_char(copy, 28, 28);
    // not a char so see type of space
    else{
        // space between letter (ref = 3)
        if(len < 5){
            pxl = SDL_MapRGB(copy->format, 3, 3, 3);
            put_pixel(copy, 0, 0, pxl);
        }
        // space between word (ref = 1)
        else{
            pxl = SDL_MapRGB(copy->format, 1, 1, 1);
            put_pixel(copy, 0, 0, pxl);
        }
    }
    return copy;
}

int charnumber(SDL_Surface *img){
	int c = 0;
	Uint32 pxl;
	Uint8 r, g, b;
	for(int i = 0; i < img->w; i++){
		pxl = get_pixel(img, i, 0);
		SDL_GetRGB(pxl, img->format, &r, &g, &b);
		if(g == 255)
			c++;
	}
	return c;
}
/*
int main(int argc, char* argv[])
{
	int compteur = 0;
    if (argc < 1)
    {
        errx(1,"must provide image");
    }

    char* img = argv[1];

    SDL_Surface* image_surface;
    SDL_Surface* image_line;
    SDL_Surface* screen_surface;
    SDL_Surface* image_char;

    init_sdl();

    image_surface = load_image(img);
    grayscale(image_surface);
    binarize(image_surface);
	screen_surface = display_image(image_surface);
	wait_for_keypressed();

	cutlines(image_surface);
	int c = linenumber(image_surface);
	screen_surface = display_image(image_surface);
	wait_for_keypressed();

	for(int i = 1; i <= c; i++)
	{
		image_line = displayline(image_surface,i);
		screen_surface = display_image(image_line);
		cutchar(image_line);
		int ch = charnumber(image_line);
		screen_surface = display_image(image_line);
		wait_for_keypressed();
		for(int i = 1; i <= ch; i++)
		{
			image_char = displaychar(image_line,i, ch);
			screen_surface = display_image(image_char);
			wait_for_keypressed();
			char str[12];
			sprintf(str,"%d",compteur+=1);
			char path[11] = "charimage/";
			strcat(path,str);
			SDL_SaveBMP(image_char,path);
		}

		wait_for_keypressed();
	}
	image_line = displayline(image_surface,3);
	cutchar(image_line);
	int ch = charnumber(image_line);
	screen_surface = display_image(image_line);
	wait_for_keypressed();

	for(int i = 1; i <= ch; i++)
	{
		image_char = displaychar(image_line,i, ch);
		screen_surface = display_image(image_char);
		wait_for_keypressed();
	}
	SDL_FreeSurface(image_surface);
	SDL_FreeSurface(screen_surface);
}*/
