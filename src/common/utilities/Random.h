//
// Created by cantte on 28/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_RANDOM_H
#define NEURONAL_NETWORKS_SERVER_RANDOM_H

/* Return a random number in the range min..max */
double d_rand(double min, double max);

struct gen_rand
{
public:
    gen_rand() : _min(-1), _max(1) {}

    gen_rand(double min, double max) : _min(min), _max(max) {}

    double operator()() const
    {
        return d_rand(_min, _max);
    }

private:
    double _min, _max;
};

#endif //NEURONAL_NETWORKS_SERVER_RANDOM_H
