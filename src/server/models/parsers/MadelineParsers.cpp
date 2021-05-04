//
// Created by cantte on 29/04/21.
//

#include "MadelineParsers.h"
#include "MultiLayerPerceptron.hpp"
#include "MadelineModels.h"

#include <boost/json.hpp>

namespace json = boost::json;

MadelineCreateInfo tag_invoke(const json::value_to_tag<MadelineCreateInfo>&, const json::value& value)
{
    uint64_t inputsNumber = value.at("InputsNumber").as_int64();

    auto neuronsPerLayer = json::value_to<std::vector<uint64_t>>(value.at("NeuronsPerLayer"));
    auto activation_functions_str = json::value_to<std::vector<std::string>>(value.at("ActivationFunctions"));

    return MadelineCreateInfo(inputsNumber, neuronsPerLayer, activation_functions_str);
}

MadelineTrainingInfo tag_invoke(const boost::json::value_to_tag<MadelineTrainingInfo>&, const boost::json::value& value)
{
    double errorTolerance = value.at("ErrorTolerance").as_double();
    double learning_rate = value.at("LearningRate").as_double();
    int maxSteps = value.at("MaxSteps").as_int64();

    auto inputs = json::value_to<std::vector<std::vector<double>>>(value.at("Inputs"));
    auto outputs = json::value_to<std::vector<std::vector<double>>>(value.at("Outputs"));

    return MadelineTrainingInfo(inputs, outputs, errorTolerance, learning_rate, maxSteps);
}

void tag_invoke(const json::value_from_tag&, json::value& jv, MultiLayerPerceptron const& m)
{
    jv = {
            {"Layers", json::value_from(m.GetLayers())}
    };
}

void tag_invoke(const boost::json::value_from_tag&, boost::json::value& jv, Layer const& layer)
{
    jv = {
            {"Nodes", json::value_from(layer.GetNodes())}
    };
}

void tag_invoke(const boost::json::value_from_tag&, boost::json::value& jv, Node const& node)
{
    jv = {
            {"Weights", json::value_from(node.GetWeights())},
            {"Sill",    node.GetBias()}
    };
}
