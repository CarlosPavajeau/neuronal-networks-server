//
// Created by cantte on 28/04/21.
//

#include "Neuron.hpp"
#include "TriggerFunction.hpp"
#include "Random.h"

Neuron::Neuron(const NeuronInitInfo& neuronInitInfo) : _bias(0), _previous_bias(0), _isLock(false), _isVisited(false),
                                                       _error(0), _newError(0)
{
    _weights = std::vector<double>(neuronInitInfo.InputNumber);
    _triggerFunction = neuronInitInfo.Trigger;

    Init();
}

void Neuron::Init()
{
    for (double& weight : _weights)
    {
        weight = drand(-1, 1);
    }

    _bias = drand(-1, 1);
}

void Neuron::UpdateWeights(std::vector<double> inputs, double error, double learning_rate)
{
    if (inputs.size() != _weights.size())
    {
        return;
    }

    _previous_bias = _bias;
    _previousWeights = _weights;

    for (int i = 0; i < _weights.size(); ++i)
    {
        _weights[i] += learning_rate * error * inputs[i];
    }

    _bias = learning_rate * error;
}

double Neuron::Output(std::vector<double> inputs)
{
    if (_triggerFunction == nullptr)
    {
        return 0;
    }

    return _triggerFunction->Output(GetNextInput(inputs));
}

double Neuron::GetNextInput(std::vector<double> inputs)
{
    double acc = 0.0;
    for (int i = 0; i < _weights.size(); ++i)
    {
        acc += _weights[i] * inputs[i];
    }

    return acc + _bias;
}

void Neuron::ResetWeightsAndSill()
{
    _weights = _previousWeights;
    _bias = _previous_bias;
}
