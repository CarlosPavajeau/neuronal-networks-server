//
// Created by cantte on 27/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_MADELINEMODELS_H
#define NEURONAL_NETWORKS_SERVER_MADELINEMODELS_H

#include "TriggerFunction.hpp"
#include <vector>

struct MadelineCreateInfo
{
public:
    MadelineCreateInfo(uint64_t inputs_number, const std::vector<uint64_t>& neurons_per_layer,
                       const std::vector<std::string>& activation_functions)
    {
        InputNumbers = inputs_number;
        NeuronsPerLayer = neurons_per_layer;
        ActivationFunctions = activation_functions;
    }

    uint64_t InputNumbers;
    std::vector<uint64_t> NeuronsPerLayer;
    std::vector<std::string> ActivationFunctions;
};

struct MadelineTrainingInfo
{
public:
    MadelineTrainingInfo(std::vector<std::vector<double>> inputs, std::vector<std::vector<double>> outputs,
                         double error_tolerance, double learning_rate, int max_steps)
    {
        Inputs = std::move(inputs);
        Outputs = std::move(outputs);

        ErrorTolerance = error_tolerance;
        LearningRate = learning_rate;
        MaxSteps = max_steps;
    }

    std::vector<std::vector<double>> Inputs;
    std::vector<std::vector<double>> Outputs;
    double ErrorTolerance;
    double LearningRate;
    int MaxSteps;
};

#endif //NEURONAL_NETWORKS_SERVER_MADELINEMODELS_H
