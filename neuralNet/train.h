#ifndef _TRAIN_H_
#define _TRAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../segmentation/sdl_operations.h"

struct train
{
    double *in; //      => Array of inputs data

    double *out; //     => Array of outputs data

    size_t nb_set; //   => Number of set data

    size_t nb_in; //    => Number of input data
    size_t nb_out; //   => Number of output data
};

// Return input of index i
double* train_in(struct train *t, size_t i);

// Return output of index i
double* train_out(struct train *t, size_t i);

// Sigmoid function
// Using exp() function from <math.h>
double sig(double x);

// Primitive function of sigmoid
double sigprime(double x);

// Load training datas from images for ocr
struct train load_ocr_train(char *dir);

// Free memory from struct train
void free_train(struct train t);
char what_is_out(double *out, size_t n);
void make_in(SDL_Surface *img, double *in);
#endif
