//
// Created by cantte on 29/04/21.
//

#include "MadelineParsers.h"
#include "Madeline.hpp"
#include "MadelineModels.h"

#include <boost/json.hpp>

namespace json = boost::json;

MadelineCreateInfo tag_invoke(const json::value_to_tag<MadelineCreateInfo>&, const json::value& value)
{
    int inputsNumber = value.at("InputsNumber").as_int64();
    double trainingRate = value.at("TrainingRate").as_double();

    auto neuronsPerLayer = json::value_to<std::vector<int>>(value.at("NeuronsPerLayer"));
    auto triggerFunctionCodes = json::value_to<std::vector<int>>(value.at("TriggerFunctions"));

    std::vector<TriggerFunction*> triggerFunctions;
    for (const auto& triggerCode: triggerFunctionCodes)
    {
        TriggerFunction* triggerFunction = sTriggerFunctionFactory->MakeTriggerFunction(
                static_cast<TriggerFunctions>(triggerCode));
        triggerFunctions.push_back(triggerFunction);
    }

    return MadelineCreateInfo(inputsNumber, neuronsPerLayer, triggerFunctions, trainingRate);
}

void tag_invoke(const json::value_from_tag&, json::value& jv, Madeline const& m)
{
    jv = {
            {"Layers", json::value_from(m.GetLayers())}
    };
}

void tag_invoke(const boost::json::value_from_tag&, boost::json::value& jv, Layer const& layer)
{
    jv = {
            {"Neurons", json::value_from(layer.GetNeurons())}
    };
}

void tag_invoke(const boost::json::value_from_tag&, boost::json::value& jv, Neuron const& neuron)
{
    jv = {
            {"Weights", json::value_from(neuron.GetWeights())},
            {"Sill",    neuron.GetSill()}
    };
}
