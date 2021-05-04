//
// Created by cantte on 28/04/21.
//

#include "Layer.hpp"
#include "Utils.h"

#include <cassert>

Layer::Layer(int num_inputs_per_node, int num_nodes, const std::string& activation_function)
{
    _num_inputs_per_node = num_inputs_per_node;
    _num_nodes = num_nodes;

    _nodes.resize(num_nodes);

    for (int i = 0; i < num_nodes; ++i)
    {
        _nodes[i].WeightInitialization(num_inputs_per_node);
    }

    std::pair<std::function<double(double)>, std::function<double(double)> >* pair;
    bool ret_val = sActivationFunctionMgr.GetActivationFunctionPair(activation_function, &pair);

    assert(ret_val);

    _activation_function = (*pair).first;
    _d_activation_function = (*pair).second;

    activation_function_str = activation_function;
}

Layer::~Layer()
{
    _num_inputs_per_node = 0;
    _num_nodes = 0;
    _nodes.clear();
}

void Layer::UpdateWeights(const std::vector<double>& input_layer_activation, const std::vector<double>& d_error,
                          double learning_rate, std::vector<double>* deltas)
{
    assert(input_layer_activation.size() == _num_inputs_per_node);
    assert(d_error.size() == _nodes.size());

    deltas->resize(_num_inputs_per_node, 0);

    for (size_t i = 0; i < _nodes.size(); ++i)
    {
        double net_sum = 0.0;
        _nodes[i].GetInputInnerProdWithWeights(input_layer_activation, &net_sum);

        double dE_doj;
        double doj_dnetj;
        double dnetj_dwij;

        dE_doj = d_error[i];
        doj_dnetj = _d_activation_function(net_sum);

        for (size_t j = 0; j < _num_inputs_per_node; ++j)
        {
            (*deltas)[j] += dE_doj * doj_dnetj * _nodes[i].GetWeights()[j];

            dnetj_dwij = input_layer_activation[j];
            _nodes[i].UpdateWeight(j, -(dE_doj * doj_dnetj * dnetj_dwij), learning_rate);
        }
    }
}

void Layer::GetOutputAfterActivationFunction(const std::vector<double>& input, std::vector<double>* output) const
{
    assert(input.size() == _num_inputs_per_node);

    output->resize(_num_nodes);

    for (size_t i = 0; i < _num_nodes; ++i)
    {
        _nodes[i].GetOutputAfterActivationFunction(input, _activation_function, &((*output)[i]));
    }
}

void Layer::SetWeights(std::vector<std::vector<double>>& weights)
{
    if (0 <= weights.size() && weights.size() <= _num_nodes)
    {
        size_t node_i = 0;
        for (Node& node : _nodes)
        {
            node.SetWeights(weights[node_i]);
            ++node_i;
        }
    } else
    {
        throw std::logic_error("Incorrect layer number in SetWeights call");
    }
}
