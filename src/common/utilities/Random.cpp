//
// Created by cantte on 28/04/21.
//

#include "Random.h"

#include <random>

static std::mt19937 engine;

double d_rand(double min, double max)
{
    std::uniform_real_distribution<double> uid(min, max);
    return uid(engine);
};
