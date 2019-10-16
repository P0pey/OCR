#ifndef _LAYER_H_
#define _LAYER_H_

#include <stdio.h>
#include "train.h"

struct layer
{
    size_t nb_neurons; //   => Number of neurons
    size_t w_per_neuron; // => Number of weights for each neurons

    double *w; //           => Array of weights
    double *previous_dw; // => Array of weights before changement
    double *bias; //        => Array of bias
    double *err; //         => Array of errors
    double *out; //         => Array of outputs
};

// Calculate index of desired weight
// With index of weight, index of neurons and checking w_per_neuron
// of actual layer
size_t get_w(struct layer *l, size_t i_n, size_t i_w);

// Initialize layers of neurons
// Allocating memory for each arrays
// Values for err and previous_dw set to 0 due to no training
// Value for bias and weigths set randomly
void initialize_layer(struct layer *l, size_t nb_neurons, size_t w_per_neuron);

// Free the memory used by layers
// Freeing each arrays
// Function called by free_network()
void free_layer(struct layer *l);

// Calculating output for layer
// Using sigmoid function with sum of weigths plus bias
void calculate_output(struct layer *l, double *inputs);

#endif
