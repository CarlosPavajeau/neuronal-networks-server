//
// Created by cantte on 28/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_LAYER_HPP
#define NEURONAL_NETWORKS_SERVER_LAYER_HPP

#include "Node.h"

#include <list>
#include <vector>

class TriggerFunction;

struct LayerCreateInfo
{
public:
    int InputsNumber;
    int NeuronsNumber;
    std::string ActivationFunction;
};

class Layer
{
public:
    explicit Layer(int num_inputs_per_node, int num_nodes, const std::string& activation_function);

    ~Layer();

    size_t GetInputSize() const
    {
        return _num_inputs_per_node;
    }

    size_t GetOutputSize() const
    {
        return _num_nodes;
    }

    void GetOutputAfterActivationFunction(const std::vector<double>& input, std::vector<double>* output) const;

    const std::vector<Node>& GetNodes() const { return _nodes; }

    std::vector<Node>& GetNodesChangeable() { return _nodes; }

    Node& operator[](int index)
    {
        return _nodes[index];
    }

    void UpdateWeights(const std::vector<double>& input_layer_activation, const std::vector<double>& d_error,
                       double learning_rate, std::vector<double>* deltas);

    void SetWeights(std::vector<std::vector<double>>& weights);

protected:
    size_t _num_inputs_per_node;
    size_t _num_nodes;

    std::vector<Node> _nodes;

    std::string activation_function_str;
    std::function<double(double)> _activation_function;
    std::function<double(double)> _d_activation_function;
};


#endif //NEURONAL_NETWORKS_SERVER_LAYER_HPP
