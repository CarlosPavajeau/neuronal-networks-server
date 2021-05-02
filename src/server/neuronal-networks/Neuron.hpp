//
// Created by cantte on 28/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_NEURON_HPP
#define NEURONAL_NETWORKS_SERVER_NEURON_HPP

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

    Neuron() {}

    void Learn(std::vector<double> inputs, double error);

    double Output(std::vector<double> inputs);

    void ResetWeightsAndSill();

    std::vector<double> GetWeights() const { return _weights; }

    double GetSill() const { return _sill; }

    bool IsLock() const { return _isLock; }

    void Lock() { _isLock = true; }

    void UnLock() { _isLock = false; }

    bool IsVisited() const { return _isVisited; }

    void Visited() { _isVisited = true; }

    void UnVisited() { _isVisited = false; }

    double GetError() const { return _error; }

    void SetError(double new_error) { _error = new_error; }

    double GetNewError() const { return _newError; }

    void SetNewError(double new_error) { _newError = new_error; }

private:
    void Init();

    double GetNextInput(std::vector<double> inputs);

    double _sill, _previousSill;
    std::vector<double> _weights, _previousWeights;
    TriggerFunction* _triggerFunction;
    double _trainingRate;

    bool _isLock, _isVisited;

    double _error, _newError;
};


#endif //NEURONAL_NETWORKS_SERVER_NEURON_HPP
