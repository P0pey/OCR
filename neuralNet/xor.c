#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "neural_network.h"

int main(int argc, char *argv[])
{
    if(argc > 2 )
        errx(1, "usage: ./xor <number of training>\n          ./xor");
    size_t nb_trains = 1000;
    if(argc == 2)
    {
        unsigned long param = strtoul(argv[1], NULL, 10);
        if(param == 0)
            errx(1,"usage: ./xor <number of training>\n            ./xor");
        nb_trains = param * 1000;
    }

    char s[] = "XOR";
    size_t n = 3;
    size_t layers[] = { 2, 2, 1 };

    struct train train = { NULL, NULL, 4, 2, 1 };
    double in[] = { 0, 0, 0, 1, 1, 0, 1, 1};
    double out[] = { 0, 1, 1, 0};
    train.in = in;
    train.out = out;

    struct network net = initialize_network(n, layers);

    for(size_t i = 0; i <= nb_trains; i++)
    {
        if(i % 100 == 0)
            printf("epoch = %lu\n", i);
        train_network(net, train, (i % 100 == 0), s);
    }

    save_network(net, "tmp/xor.nn");
    printf("save succed in tmp/xor.nn\n"); 

    struct network net_loaded = load_network("tmp/xor.nn");
    printf("load succed in tmp/xor.nn\n");

    double inputs[] = {0 , 0};
    scanf("%lf %lf", &inputs[0], &inputs[1]);
    compute_network(net, inputs);

    double result1 = net.layers[net.nb_layers - 1].out[0];
    result1 = (net.layers[net.nb_layers - 1].out[0] > 0.5f) ? 1 : 0;

    printf("%.1f XOR %.1f = %f <=> %.1f\n", inputs[0], inputs[1], 
            net.layers[net.nb_layers - 1].out[0], result1);

    scanf("%lf %lf", &inputs[0], &inputs[1]);
    compute_network(net_loaded, inputs);

    double result2 = net_loaded.layers[net_loaded.nb_layers - 1].out[0];
    result2 = (net_loaded.layers[net_loaded.nb_layers - 1].out[0] > 0.5f) ? 1 : 0;

    printf("%.1f XOR %.1f = %f <=> %.1f\n", inputs[0], inputs[1], 
            net_loaded.layers[net_loaded.nb_layers - 1].out[0], result2);

    free_network(net_loaded);
    free_network(net);
    return 0;
}
