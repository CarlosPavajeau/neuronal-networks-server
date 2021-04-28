//
// Created by cantte on 27/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_MADELINEMODELS_H
#define NEURONAL_NETWORKS_SERVER_MADELINEMODELS_H

#include "TriggerFunction.hpp"
#include <vector>

namespace boost
{
    namespace json
    {
        template<typename T>
        class value_to_tag;

        class value;
    }
}

struct MadelineCreateInfo
{
public:
    MadelineCreateInfo() = default;

    MadelineCreateInfo(int inputNumbers, std::vector<int> neuronsPerLayer, std::vector<TriggerFunction*> triggers,
                       double trainingRate) : InputNumbers(inputNumbers), NeuronsPerLayer(std::move(neuronsPerLayer)),
                                              Triggers(std::move(triggers)), TrainingRate(trainingRate) {}

    int InputNumbers{};
    std::vector<int> NeuronsPerLayer;
    std::vector<TriggerFunction*> Triggers;
    double TrainingRate{};
};

struct MadelineTrainingInfo
{
public:
    std::vector<std::vector<double>> Inputs;
    std::vector<std::vector<double>> Outputs;
    double ErrorTolerance;
    int MaxSteps;
};

MadelineCreateInfo tag_invoke(const boost::json::value_to_tag<MadelineCreateInfo>&, const boost::json::value& value);

#endif //NEURONAL_NETWORKS_SERVER_MADELINEMODELS_H
