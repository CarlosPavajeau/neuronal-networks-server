//
// Created by cantte on 28/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_NEURON_H
#define NEURONAL_NETWORKS_SERVER_NEURON_H

#include <vector>
#include <random>

class TriggerFunction;

struct NeuronInitInfo
{
public:
    int InputNumber;
    double TrainingRate;
    TriggerFunction* Trigger;
};

class Neuron
{
public:
    explicit Neuron(const NeuronInitInfo& neuronInitInfo);

    void Learn(std::vector<double> inputs, double error);

    double Output(std::vector<double> inputs);

    void ResetWeightsAndSill();

private:
    void Init();

    double GetNextInput(std::vector<double> inputs);

    double _sill, _previusSill;
    std::vector<double> _weights, _previusWeights;
    TriggerFunction* _triggerFunction;
    double _trainingRate;

    bool _isLock, _isVisited;

    double _error, _newError;
};


#endif //NEURONAL_NETWORKS_SERVER_NEURON_H
