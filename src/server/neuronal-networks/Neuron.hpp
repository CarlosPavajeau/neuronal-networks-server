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
    TriggerFunction* Trigger;
};

class Neuron
{
public:
    explicit Neuron(const NeuronInitInfo& neuronInitInfo);

    Neuron() = default;

    void UpdateWeights(std::vector<double> inputs, double error, double learning_rate);

    double Output(std::vector<double> inputs);

    void ResetWeightsAndSill();

    std::vector<double> GetWeights() const { return _weights; }

    double GetBias() const { return _bias; }

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

    double _bias{}, _previous_bias{};
    std::vector<double> _weights, _previousWeights;
    TriggerFunction* _triggerFunction{};

    bool _isLock{}, _isVisited{};

    double _error{}, _newError{};
};


#endif //NEURONAL_NETWORKS_SERVER_NEURON_HPP
