#include "layer.h"

size_t get_w(struct layer *l, size_t i_n, size_t i_w)
{
    return i_w + i_n * l->w_per_neuron;
}

void initialize_layer(struct layer *l, size_t nb_neurons, size_t w_per_neuron)
{
    l->nb_neurons = nb_neurons;
    l->w_per_neuron = w_per_neuron;

    //If not input layer
    if(w_per_neuron)
    {
        l->w = malloc(sizeof(double) * nb_neurons * w_per_neuron);
        l->previous_dw = calloc(sizeof(double), nb_neurons * w_per_neuron);
        l->bias = malloc(sizeof(double) * nb_neurons);
        l->err = calloc(sizeof(double), nb_neurons);

        for(size_t i = 0; i < nb_neurons; i++)
        {
            l->bias[i] = -1. + 2. * ((double)rand() / RAND_MAX);
            for(size_t j = 0; j < w_per_neuron; j++)
                l->w[get_w(l, i ,j)] = -1. + 2. * ((double)rand() / RAND_MAX);
        }
    }
    else
    {
        l->w = NULL;
        l->previous_dw = NULL;
        l->bias = NULL;
        l->err = NULL;
    }

    l->out = malloc(sizeof(double) * nb_neurons);
}

void free_layer(struct layer *l)
{
    free(l->w);
    free(l->previous_dw);
    free(l->bias);
    free(l->err);
    free(l->out);
}

void calculate_output(struct layer *l, double *inputs)
{
    for(size_t i = 0; i < l->nb_neurons; i++)
    {
        double si = 0;
        for(size_t j = 0; j < l->w_per_neuron; j++)
            si += l->w[get_w(l, i, j)] * inputs[j];
        l->out[i] = sig(si + l->bias[i]);
    }
}
