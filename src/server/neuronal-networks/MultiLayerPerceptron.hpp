//
// Created by cantte on 28/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_MULTILAYER_PERCEPTRON_H
#define NEURONAL_NETWORKS_SERVER_MULTILAYER_PERCEPTRON_H

#include "Layer.hpp"
#include "MadelineModels.h"

#include <vector>

class Session;

class MultiLayerPerceptron
{
    friend class Session;

public:
    explicit MultiLayerPerceptron(const std::vector<uint64_t>& layers_nodes,
                                  const std::vector<std::string>& layers_act_funcs, Session* session);

    ~MultiLayerPerceptron();

    void GetOutput(const std::vector<double>& input, std::vector<double>* output,
                   std::vector<std::vector<double>>* all_layers_activations = nullptr) const;

    void GetOutputClass(const std::vector<double>& output, size_t* class_id) const;

    bool Train(const MadelineTrainingInfo& madelineTrainingInfo);

    size_t GetNumLayers() { return _layers.size(); }

    std::vector<std::vector<double>> GetLayerWeights(size_t layer_i);

    void SetLayerWeights(size_t layer_i, std::vector<std::vector<double>>& weights);

    std::vector<Layer> GetLayers() const { return _layers; }

protected:
    void UpdateWeights(const std::vector<std::vector<double>>& all_layers_activations, const std::vector<double>& error,
                       double learning_rate);

private:
    void Init(const std::vector<uint64_t>& layers_nodes, const std::vector<std::string>& layers_act_funcs);

    size_t _num_inputs;
    int _num_outputs, _num_hidden_layers;

    std::vector<uint64_t> _layer_nodes;
    std::vector<Layer> _layers;
    Session* _session;
};

#endif //NEURONAL_NETWORKS_SERVER_MULTILAYER_PERCEPTRON_Hs
