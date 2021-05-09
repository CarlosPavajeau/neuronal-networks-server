//
// Created by cantte on 8/05/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_RADIALBASISFUNCTIONMODELS_H
#define NEURONAL_NETWORKS_SERVER_RADIALBASISFUNCTIONMODELS_H

#include <vector>

struct RadialBasisFunctionCreateInfo
{
public:
    RadialBasisFunctionCreateInfo(uint64 inputs_number, uint64 radial_centers)
    {
        InputNumbers = inputs_number;
        RadialCenters = radial_centers;
    }

    uint64 InputNumbers;
    uint64 RadialCenters;
};

struct RadialBasisFunctionTrainingInfo
{
public:
    RadialBasisFunctionTrainingInfo(const std::vector<std::vector<double>>& inputs, const std::vector<double>& outputs,
                                    double error_tolerance, int max_steps)
    {
        Inputs = inputs;
        Outputs = outputs;
        ErrorTolerance = error_tolerance;
        MaxSteps = max_steps;
    }

    std::vector<std::vector<double>> Inputs;
    std::vector<double> Outputs;
    double ErrorTolerance;
    int MaxSteps;
};

#endif //NEURONAL_NETWORKS_SERVER_RADIALBASISFUNCTIONMODELS_H
