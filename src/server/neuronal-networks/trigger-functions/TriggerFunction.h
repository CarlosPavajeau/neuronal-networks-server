//
// Created by cantte on 28/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_TRIGGERFUNCTION_H
#define NEURONAL_NETWORKS_SERVER_TRIGGERFUNCTION_H

#include <cmath>

enum TriggerFunctions
{
    Linear,
    Sigmoidal,
    HyperbolicTangent
};

class TriggerFunction
{
public:
    virtual double Output(double input) = 0;
};

class LinearFunction final : TriggerFunction
{
public:
    double Output(double input) override
    {
        return input;
    }
};

class SigmoidalFunction final : TriggerFunction
{
public:
    double Output(double input) override
    {
        return 1 / (1 + std::exp(-2 * input));
    }
};

class HyperbolicTangent final : TriggerFunction
{
public:
    double Output(double input) override
    {
        return std::tanh(input);
    }
};

#endif //NEURONAL_NETWORKS_SERVER_TRIGGERFUNCTION_H
