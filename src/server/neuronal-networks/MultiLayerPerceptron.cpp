//
// Created by cantte on 28/04/21.
//

#include "MultiLayerPerceptron.hpp"
#include "Session.hpp"
#include "Utils.h"

#include <algorithm>

MultiLayerPerceptron::MultiLayerPerceptron(const std::vector<uint64_t>& layers_nodes, const std::vector<std::string>& layers_act_funcs,
                   Session* session) : _session(session)
{
    assert(layers_nodes.size() >= 2);
    assert(layers_act_funcs.size() + 1 == layers_nodes.size());

    Init(layers_nodes, layers_act_funcs);
}

MultiLayerPerceptron::~MultiLayerPerceptron()
{
    _num_inputs = 0;
    _num_outputs = 0;
    _num_hidden_layers = 0;
    _layer_nodes.clear();
    _layers.clear();
}

void MultiLayerPerceptron::Init(const std::vector<uint64_t>& layers_nodes, const std::vector<std::string>& layers_act_funcs)
{
    _layer_nodes = layers_nodes;
    _num_inputs = _layer_nodes[0];
    _num_inputs = _layer_nodes.back();
    _num_hidden_layers = _layer_nodes.size() - 2;

    for (size_t i = 0; i < _layer_nodes.size() - 1; ++i)
    {
        _layers.emplace_back(Layer(_layer_nodes[i], _layer_nodes[i + 1], layers_act_funcs[i]));
    }
}

void MultiLayerPerceptron::GetOutput(const std::vector<double>& input, std::vector<double>* output,
                         std::vector<std::vector<double>>* all_layers_activations) const
{
    assert(input.size() == _num_inputs);

    int temp_size;
    if (_num_hidden_layers == 0)
    {
        temp_size = _num_outputs;
    } else
    {
        temp_size = _layer_nodes[1];
    }

    std::vector<double> temp_in(_num_inputs, 0.0);
    std::vector<double> temp_out(temp_size, 0.0);

    temp_in = input;

    for (size_t i = 0; i < _layers.size(); ++i)
    {
        if (i > 0)
        {
            if (all_layers_activations != nullptr)
            {
                all_layers_activations->emplace_back(std::move(temp_in));
            }

            temp_in.clear();
            temp_in = temp_out;
            temp_out.clear();
            temp_out.resize(_layers[i].GetOutputSize());
        }

        _layers[i].GetOutputAfterActivationFunction(temp_in, &temp_out);
    }

    if (temp_out.size() > 1)
    {
        Softmax(&temp_out);
    }

    *output = temp_out;

    if (all_layers_activations != nullptr)
    {
        all_layers_activations->emplace_back(std::move(temp_in));
    }
}

void MultiLayerPerceptron::GetOutputClass(const std::vector<double>& output, size_t* class_id) const
{
    GetIdMaxElement(output, class_id);
}

void MultiLayerPerceptron::UpdateWeights(const std::vector<std::vector<double>>& all_layers_activations,
                             const std::vector<double>& error, double learning_rate)
{
    std::vector<double> temp_dev_error = error;
    std::vector<double> deltas{};

    for (int i = _num_hidden_layers; i >= 0; --i)
    {
        _layers[i].UpdateWeights(all_layers_activations[i], temp_dev_error, learning_rate, &deltas);
        if (i > 0)
        {
            temp_dev_error.clear();
            temp_dev_error = std::move(deltas);
            deltas.clear();
        }
    }
}

bool MultiLayerPerceptron::Train(const MadelineTrainingInfo& madelineTrainingInfo)
{
    int i = 0;
    double current_iteration_cost_function = 0.0;

    for (i = 0; i < madelineTrainingInfo.MaxSteps; ++i)
    {
        current_iteration_cost_function = 0.0;

        for (size_t j = 0; j < madelineTrainingInfo.Inputs.size(); ++j)
        {
            std::vector<double> predicted_output;
            std::vector<std::vector<double>> all_layers_activations;

            GetOutput(madelineTrainingInfo.Inputs[i], &predicted_output, &all_layers_activations);

            const std::vector<double>& correct_output = madelineTrainingInfo.Outputs[i];

            assert(correct_output.size() == predicted_output.size());

            std::vector<double> d_error_output(predicted_output.size());
            for (size_t k = 0; k < predicted_output.size(); ++k)
            {
                current_iteration_cost_function += std::pow(correct_output[k] - predicted_output[j], 2);
                d_error_output[k] = -2 * (correct_output[k] - predicted_output[k]);
            }

            UpdateWeights(all_layers_activations, d_error_output, madelineTrainingInfo.LearningRate);
        }

        if (current_iteration_cost_function < madelineTrainingInfo.ErrorTolerance)
        {
            return true;
        }
    }

    return false;
}

std::vector<std::vector<double>> MultiLayerPerceptron::GetLayerWeights(size_t layer_i)
{
    std::vector<std::vector<double>> ret_val;

    if (0 <= layer_i && layer_i < _layer_nodes.size())
    {
        Layer current_layer = _layers[layer_i];
        for (Node& node : current_layer.GetNodesChangeable())
        {
            ret_val.push_back(node.GetWeights());
        }

        return ret_val;
    } else
    {
        throw std::logic_error("Incorrect layer number in GetLayerWeights call");
    }
}

void MultiLayerPerceptron::SetLayerWeights(size_t layer_i, std::vector<std::vector<double>>& weights)
{
    if (0 <= layer_i && layer_i < _layers.size())
    {
        _layers[layer_i].SetWeights(weights);
    } else
    {
        throw std::logic_error("Incorrect layer number in SetLayerWeights call");
    }
}