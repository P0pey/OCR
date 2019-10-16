#include "train.h"

double* train_in(struct train *t, size_t i)
{
    return &t->in[i * t->nb_in];
}

double* train_out(struct train *t, size_t i)
{
    return &t->out[i * t->nb_out];
}

double sig(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

double sigprime(double x)
{
    return (1.0 - x) * x;
}

void make_in(SDL_Surface *img, double *in)
{
    Uint32 pixel;
    Uint8 r, g , b;
    for(int i = 0; i < img->w; i++)
    {
        for(int j = 0; j < img->h; j++)
        {
            pixel = get_pixel(img,i,j);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            in[j + i * img->w] = r > 127 ? 1 : 0;
        }
    }
}

void make_out(double *out, size_t n, char c)
{
    for(size_t i = 0; i < n; i++)
        out[i] = 0;
    if('0' <= c && c <= '9')
        out[c-'0'] = 1;
    else if('a' <= c && c <= 'z')
        out[10+c-'a'] = 1;
    else
        out[36+c-'A'] = 1;
}

char what_is_out(double *out, size_t n)
{
    size_t i = 0;
    while(i < n && out[i] != 1)
        i++;
    if(i < 10)
        return '0' + i;
    else if(10 <= i && i < 36)
        return 'a' + (i - 10);
    else if(i < n)
        return 'A' + (i - 36);
    else
        return '\0';
}

struct train load_ocr_train(char *dir)
{
    size_t nb_tries = 40;
    size_t nb_letters = 62;
    size_t nb_set = nb_tries * nb_letters;
    size_t nb_in = 784;
    size_t nb_out = 62;

    struct train t = { NULL, NULL, nb_set, nb_in, nb_out};
    t.in = malloc(sizeof(double) * t.nb_in * t.nb_set);
    t.out = malloc(sizeof(double) * t.nb_out * t.nb_set);

    SDL_Surface *img;
    char file[4096];
    for(size_t try = 1; try <= nb_tries; try++)
    {
        for(size_t i = 0; i < 10; i++)
        {
            sprintf(file, "%sdigit/%c/%lu.png", dir, (char)i+'0', try);
            img = load_image(file); 
            printf("Load -> %s w:%i h:%i\n", file, img->w, img->h);
            binarize(img);
            size_t use = i + (try - 1) * nb_out;
            make_in(img, train_in(&t, use));
            make_out(train_out(&t, use), nb_out, (char)i+'0');
        }

        for(size_t i = 0; i < 26; i++)
        {
            sprintf(file, "%slower/%c/%lu.png", dir, (char)i+'a', try);
            img = load_image(file); 
            printf("Load -> %s w:%i h:%i\n", file, img->w, img->h);
            binarize(img);
            size_t use = (i + 10) + (try - 1) * nb_out;
            make_in(img, train_in(&t, use));
            make_out(train_out(&t, use), nb_out, (char)i+'a');
        }

        for(size_t i = 0; i < 26; i++)
        {
            sprintf(file, "%supper/%c/%lu.png", dir, (char)i+'a', try);
            img = load_image(file); 
            printf("Load -> %s w:%i h:%i\n", file, img->w, img->h);
            binarize(img);
            size_t use = (i + 36) + (try - 1) * nb_out;
            make_in(img, train_in(&t, use));
            make_out(train_out(&t, use), nb_out, (char)i+'A');
        }
    }
    return t;
}

void free_train(struct train t)
{
    free(t.in);
    free(t.out);
    t.nb_set = 0;
    t.nb_in = 0;
    t.nb_out = 0;
}
