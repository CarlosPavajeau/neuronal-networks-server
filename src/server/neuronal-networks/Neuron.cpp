//
// Created by cantte on 28/04/21.
//

#include "Neuron.hpp"
#include "TriggerFunction.hpp"
#include "Random.h"

Neuron::Neuron(const NeuronInitInfo& neuronInitInfo) : _sill(0), _previousSill(0), _isLock(false), _isVisited(false),
                                                       _error(0), _newError(0)
{
    _weights = std::vector<double>(neuronInitInfo.InputNumber);
    _trainingRate = neuronInitInfo.TrainingRate;
    _triggerFunction = neuronInitInfo.Trigger;

    Init();
}

void Neuron::Init()
{
    for (double& weight : _weights)
    {
        weight = drand(-1, 1);
    }

    _sill = drand(-1, 1);
}

void Neuron::Learn(std::vector<double> inputs, double error)
{
    if (inputs.size() != _weights.size())
    {
        return;
    }

    _previousSill = _sill;
    _previousWeights = _weights;

    for (int i = 0; i < _weights.size(); ++i)
    {
        _weights[i] += _trainingRate * error * inputs[i];
    }

    _sill = _trainingRate * error;
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

    return acc + _sill;
}

void Neuron::ResetWeightsAndSill()
{
    _weights = _previousWeights;
    _sill = _previousSill;
}
