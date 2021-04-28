//
// Created by cantte on 28/04/21.
//

#include "MadelineModels.h"

#include <boost/json/src.hpp>

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

