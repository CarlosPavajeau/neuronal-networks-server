//
// Created by cantte on 5/05/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_SELFORGMAPMODELS_H
#define NEURONAL_NETWORKS_SERVER_SELFORGMAPMODELS_H

#include "Define.hpp"

#include <vector>

struct SelfOrgMapCreateInfo
{
public:
    SelfOrgMapCreateInfo(uint64 width, uint64 height, int num_inputs, double delta, bool tough_competition)
    {
        Width = width;
        Height = height;
        InputNumbers = num_inputs;
        Delta = delta;
        ToughCompetition = tough_competition;
    }

    uint64 Width, Height;
    int InputNumbers;
    double Delta;
    bool ToughCompetition;
};

struct SelfOrgMapTrainingInfo
{
public:
    SelfOrgMapTrainingInfo(std::vector<std::vector<double>> inputs, double error_tolerance, double learning_rate,
                           int max_steps)
    {
        Inputs = std::move(inputs);
        ErrorTolerance = error_tolerance;
        LearningRate = learning_rate;
        MaxSteps = max_steps;
    }

    std::vector<std::vector<double>> Inputs;
    double ErrorTolerance;
    double LearningRate;
    int MaxSteps;
};

#endif //NEURONAL_NETWORKS_SERVER_SELFORGMAPMODELS_H
