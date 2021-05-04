//
// Created by cantte on 28/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_RANDOM_H
#define NEURONAL_NETWORKS_SERVER_RANDOM_H

/* Return a random number in the range min..max */
double drand(double min, double max);

struct gen_rand
{
public:
    double operator()()
    {
        return drand(-1, 1);
    }
};

#endif //NEURONAL_NETWORKS_SERVER_RANDOM_H
