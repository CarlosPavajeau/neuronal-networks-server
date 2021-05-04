//
// Created by cantte on 3/05/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_NODE_H
#define NEURONAL_NETWORKS_SERVER_NODE_H

#include <algorithm>
#include <vector>
#include <stdexcept>
#include <functional>

class Node
{
public:
    Node();

    explicit Node(int num_inputs);

    ~Node();

    void WeightInitialization(int nun_inputs);

    size_t GetInputSize() const { return _num_inputs; }

    void SetInputSize(int num_inputs) { _num_inputs = num_inputs; }

    double GetBias() const { return _bias; }

    void SetBias(double bias) { _bias = bias; }

    std::vector<double>& GetWeights()
    {
        return _weights;
    }

    const std::vector<double>& GetWeights() const
    {
        return _weights;
    }

    void SetWeights(std::vector<double>& weights)
    {
        // check size of the weights vector
        if (weights.size() == _num_inputs)
            _weights = weights;
        else
        {
            throw std::logic_error("Incorrect weight size in SetWeights call");
        }
    }

    size_t GetWeightsVectorSize() const
    {
        return _weights.size();
    }

    void GetInputInnerProdWithWeights(const std::vector<double>& input, double* output) const;

    void GetOutputAfterActivationFunction(const std::vector<double>& input,
                                          std::function<double(double)> activation_function,
                                          double* output) const;

    void UpdateWeights(const std::vector<double>& x, double error, double learning_rate);

    void UpdateWeight(int weight_id, double increment, double learning_rate);

protected:
    size_t _num_inputs;
    double _bias;
    std::vector<double> _weights;
};


#endif //NEURONAL_NETWORKS_SERVER_NODE_H
