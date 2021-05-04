//
// Created by cantte on 3/05/21.
//

#include "Node.h"
#include "Random.h"

#include <cassert>
#include <numeric>

Node::Node() : _num_inputs(0)
{
    _bias = 0.0;
    _weights.clear();
}

Node::~Node()
{
    _num_inputs = 0;
    _bias = 0;
    _weights.clear();
}

Node::Node(int num_inputs) : _num_inputs(num_inputs)
{
    _bias = 0.0;
    _weights.clear();
    WeightInitialization(num_inputs);
}

void Node::WeightInitialization(int nun_inputs)
{
    _num_inputs = nun_inputs;
    _weights.resize(_num_inputs);

    std::generate_n(_weights.begin(), _num_inputs, gen_rand());
}

void Node::GetInputInnerProdWithWeights(const std::vector<double>& input, double* output) const
{
    assert(input.size() == _weights.size());

    double inner_prod = std::inner_product(input.begin(), input.end(), _weights.begin(), 0.0);

    *output = inner_prod;
}

void Node::GetOutputAfterActivationFunction(const std::vector<double>& input,
                                            std::function<double(double)> activation_function, double* output) const
{
    double inner_prod = 0.0;
    GetInputInnerProdWithWeights(input, &inner_prod);
    *output = activation_function(inner_prod);
}

void Node::UpdateWeights(const std::vector<double>& x, double error, double learning_rate)
{
    assert(x.size() == _weights.size());
    for (size_t i = 0; i < _weights.size(); ++i)
    {
        _weights[i] += x[i] * learning_rate * error;
    }
}

void Node::UpdateWeight(int weight_id, double increment, double learning_rate)
{
    _weights[weight_id] += learning_rate * increment;
}
