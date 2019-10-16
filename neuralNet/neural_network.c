#include "neural_network.h"
#include <err.h>
#include <stdlib.h>
#include <stdio.h>

struct network initialize_network(size_t nb_layers, size_t *nb_neurons_layer)
{
    struct network net;
    net.nb_layers = nb_layers;

    struct layer *l;
    l = malloc(sizeof(struct layer) * nb_layers);
    srand(time(NULL));
    initialize_layer(&l[0], nb_neurons_layer[0], 0);
    for(size_t i = 1; i < nb_layers; i++)
        initialize_layer(&l[i], nb_neurons_layer[i], nb_neurons_layer[i-1]);
    net.layers = l;

    return net;
}

void free_network(struct network net)
{
    for(size_t i = 0; i < net.nb_layers; i++)
        free_layer(net.layers + i);
    net.nb_layers = 0;
    free(net.layers);
}

void compute_network(struct network net, double *inputs)
{
    for(size_t i = 0; i < net.layers[0].nb_neurons; i++)
        net.layers[0].out[i] = inputs[i];
    for(size_t i = 1; i < net.nb_layers; i++)
        calculate_output(&net.layers[i], net.layers[i - 1].out);
}

double* output_network(struct network net)
{
    struct layer *l = &net.layers[net.nb_layers - 1];
    for(size_t i = 0; i < l->nb_neurons; i++)
        l->out[i] = (l->out[i] > 0.5) ? 1 : 0;
    return l->out;
}

int print_ocr_train(struct network net, struct train train, size_t i)
{
    char c = what_is_out(train_out(&train, i), 62);
    char res = what_is_out(output_network(net) ,62);
    if(c != res)
    {
        printf("\033[31m");
        printf("IN : %c\t", c);
        printf("OUT : %c\n",res);
    }
    if(c != res)
        printf("\033[00m");
    return c == res;
}

void train_network(struct network net, struct train train, int p, char s[])
{
    double err = 0;
    int per = 0;
    size_t i = 0;
    for(; i < train.nb_set; i++)
    {
        compute_network(net, train_in(&train, i));
        err += err_network(net, train_out(&train, i));

        forward_prop(net);
        backward_prop(net);
        if(p)
        {
            if(s[0] == 'O')
            {
               per += print_ocr_train(net, train, i);
               if(i % 62 == 0)
                   printf("FONT n°%lu\n", i / 62 + 1);
            }
            else
                print_train(net, train, i, s);
        }
    }
    err *= (1.0 / (double)train.nb_set);
    if(p)
    {
        if(s[0] == 'O')
            printf("Progression = %d/%lu\n", per, i);
        printf("error = %.12f\n\n", err);
    }
}

double err_network(struct network net, double *goal)
{
    double err = 0;

    struct layer *last = &net.layers[net.nb_layers - 1];
    for(size_t i = 0; i < last->nb_neurons; i++)
    {
        last->err[i] = sigprime(last->out[i]) * (goal[i] - last->out[i]);
        err += pow(last->err[i], 2);
    }
    err *= 0.5;
    return err;
}

void forward_prop(struct network net)
{
    for(size_t b = net.nb_layers - 2; b > 0; b--)
    {
        struct layer *actu = &net.layers[b];
        struct layer *done = &net.layers[b + 1];
        for(size_t actun = 0; actun < actu->nb_neurons; actun++)
        {
            double sum = 0;
            for(size_t donen = 0; donen <done->nb_neurons; donen++)
                sum += done->err[donen] * done->w[get_w(done, donen, actun)];
            actu->err[actun] = sigprime(actu->out[actun]) * sum;
        }
    }
}

void backward_prop(struct network net)
{
    double l_rate = 0.4;
    for(size_t f = 1; f < net.nb_layers; f++)
    {
        struct layer *actu = &net.layers[f];
        struct layer *done = &net.layers[f - 1];
        for(size_t actun = 0; actun < actu->nb_neurons; actun++)
        {
            actu->bias[actun] += l_rate * actu->err[actun];
            for(size_t donen = 0; donen < done->nb_neurons; donen++)
            {
                size_t i = get_w(actu, actun, donen);
                double deltaw = l_rate * actu->err[actun] * done->out[donen];
                actu->w[i] += deltaw + 0.1 * actu->previous_dw[i];
                actu->previous_dw[i] = deltaw;
            }
        }
    }
}

void print_train(struct network net,struct train train, size_t i, char* s)
{
    double *arr = train_in(&train, i);
    for(size_t j = 0; j < train.nb_in - 1; j++)
        printf("%.1f %s ", arr[j], s);
    printf("%.1f ", arr[train.nb_in - 1]);
    printf("= ");
    for(size_t j = 0; j < train.nb_out; j++)
        printf("%f ", net.layers[net.nb_layers - 1].out[j]);
    printf("(expected = ");
    arr = train_out(&train, i);
    for(size_t j = 0; j < train.nb_out; j++)
        printf("%.1f", arr[j]);
    printf(")\n");
}

// Saving network in a file (give a filepath)
void save_network(struct network net, const char* filepath)
{
    FILE* myFile = fopen(filepath, "w");

    if(!myFile)
        errx(1,"Problem with filepath %s", filepath);

    // Write in file the number of layers
    fprintf(myFile, "%ld\n", net.nb_layers);
    // Jump before and after each layer
    //fprintf(myFile,"\n");

    // First Layer
    // Number of neurons
    fprintf(myFile, "%lu\n", net.layers[0].nb_neurons);
    //Number of weights per neurons
    fprintf(myFile, "%lu\n", net.layers[0].w_per_neuron);

    // Jump before hidden and output layer

    // Save datas for each layer
    for(size_t i = 1; i < net.nb_layers; i++)
    {
        //fprintf(myFile, "\n");
        struct layer myLayer = net.layers[i];

        // Number of neurons
        fprintf(myFile, "%lu\n", myLayer.nb_neurons);

        // Number of weights for each neurons
        fprintf(myFile, "%lu\n", myLayer.w_per_neuron);

        // Weights
        for(size_t j = 0; j < myLayer.nb_neurons; j++)
            for(size_t k = 0; k < myLayer.w_per_neuron; k++)
                fprintf(myFile, "%a\n", myLayer.w[get_w(&myLayer, j, k)]);

        // Bias
        for(size_t j = 0; j < myLayer.nb_neurons ; j++)
            fprintf(myFile, "%a\n", myLayer.bias[j]);

        // Jump for end of layer
    }
    fclose(myFile);
}

struct network load_network(const char* filepath)
{
    FILE* file = fopen(filepath, "r");

    if(!file)
        errx(1, "Problem with filepath: %s", filepath);

    struct network net;
    fscanf(file, "%lu", &net.nb_layers);
    net.layers = malloc(sizeof(struct layer) * net.nb_layers);

    for(size_t i = 0; i < net.nb_layers; i++)
    {
        struct layer *l = &net.layers[i];
        fscanf(file, "%lu", &l->nb_neurons);
        fscanf(file, "%lu", &l->w_per_neuron);
        if(l->w_per_neuron)
        {
            l->w = malloc(sizeof(double) * l->nb_neurons * l->w_per_neuron);
            l->previous_dw = calloc(sizeof(double), 
                    l->nb_neurons * l->w_per_neuron);
            l->bias = malloc(sizeof(double) * l->nb_neurons);
            l->err = calloc(sizeof(double), l->nb_neurons);

            for(size_t j = 0; j < l->nb_neurons; j++)
                for(size_t k = 0; k < l->w_per_neuron; k++)
                    fscanf(file, "%la", &l->w[get_w(l, j, k)]);

            for(size_t j = 0; j < l->nb_neurons; j++)
                fscanf(file, "%la", &l->bias[j]);
        }
        else
        {
            l->w = NULL;
            l->previous_dw = NULL;
            l->bias = NULL;
            l-> err = NULL;
        }
        l->out = malloc(sizeof(double) * l->nb_neurons);
    }

    fclose(file);
    return net;
}
