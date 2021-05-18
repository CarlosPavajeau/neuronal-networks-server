//
// Created by cantte on 27/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_MADELINEMODELS_H
#define NEURONAL_NETWORKS_SERVER_MADELINEMODELS_H

#include "Define.hpp"
#include <vector>

struct MadelineCreateInfo
{
public:
    MadelineCreateInfo(uint64 input_numbers, const std::vector<uint64>& neurons_per_layer,
                       const std::vector<std::string>& activation_functions)
    {
        InputNumbers = input_numbers;
        NeuronsPerLayer = neurons_per_layer;
        ActivationFunctions = activation_functions;
    }

    uint64 InputNumbers;
    std::vector<uint64> NeuronsPerLayer;
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
