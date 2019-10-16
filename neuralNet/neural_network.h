#ifndef _NEURAL_NETWORK_H_
#define _NEURAL_NETWORK_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "layer.h"
#include "train.h"

struct network
{
    size_t nb_layers; //    => Number of layers

    struct layer *layers; //=> Array of layers
};

// Initialize the network
// Allocate memories for each layers
// By calling initialize_layer()
// nb_layers have to be superior to 2 
struct network initialize_network(size_t nb_layers, size_t *nb_neurons_layer);

// Free memory used by network
// Free each layer by calling free_layer()
// Then free layers array
void free_network(struct network net);

// Run the network
// First layer taken appart with input from training
// Foreach layers from second to last calculate_output() used
void compute_network(struct network net, double *inputs);

// Training the network
// Using compute_network()
// And backward_prop() plus forward_prop()
// Calculating the error purcentage
// Print the results if p
void train_network(struct network net, struct train train, int p, char s[]);

// Calculate error produced by network
// Comparing desired results from array out from train
// Last array's err array updated
// Returned value only used for information purposes
double err_network(struct network net, double *goal);

// Forward propagation
// Calculate sum of actual and next layers' err * weigth
// This sum is multiplied by sigprime() of actual layer's out
// And assigned to actual layer's err
void forward_prop(struct network net);

// Backward propagation
// Comparing actual and previous layer's weight
// Actual bias modified with a learning rate fixed
void backward_prop(struct network net);

// Print result of training
// Called by train_network()
// Print obtained results and expected results
void print_train(struct network net, struct train train, size_t i, char* s);

// save a network in a file
void save_network(struct network net, const char* filepath);

// load a network from a file
// struct network load_network(const char* filepath);
struct network load_network(const char* filepath);

double* output_network(struct network net);

#endif
