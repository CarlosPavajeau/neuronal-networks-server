//
// Created by cantte on 28/04/21.
//

#include "Layer.hpp"

Layer::Layer(const LayerCreateInfo& layerCreateInfo)
{
    NeuronInitInfo neuronInitInfo{.InputNumber = layerCreateInfo.InputsNumber,
            .TrainingRate = layerCreateInfo.TrainingRate,
            .Trigger = layerCreateInfo.Trigger};

    for (int i = 0; i < layerCreateInfo.NeuronsNumber; ++i)
    {
        Neuron neuron(neuronInitInfo);
        _neurons.push_back(neuron);
    }
}

std::vector<double> Layer::Output(const std::vector<double>& inputs)
{
    _outputs = std::vector<double>(_neurons.size());

    for (Neuron& neuron : _neurons)
    {
        _outputs.push_back(neuron.Output(inputs));
    }

    return _outputs;
}
