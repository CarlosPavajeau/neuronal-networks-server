//
// Created by cantte on 28/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_LAYER_HPP
#define NEURONAL_NETWORKS_SERVER_LAYER_HPP

#include "Neuron.hpp"

#include <list>
#include <vector>

class TriggerFunction;

struct LayerCreateInfo
{
public:
    int InputsNumber;
    int NeuronsNumber;
    TriggerFunction* Trigger;
    double TrainingRate;
};

class Layer
{
public:
    explicit Layer(const LayerCreateInfo& layerCreateInfo);

    std::vector<double> Output(const std::vector<double>& inputs);

    std::vector<double> GetOutputs() const { return _outputs; }

private:
    std::list<Neuron> _neurons;
    std::vector<double> _outputs;
};


#endif //NEURONAL_NETWORKS_SERVER_LAYER_HPP
