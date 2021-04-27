//
// Created by cantte on 27/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_MADELINE_H
#define NEURONAL_NETWORKS_SERVER_MADELINE_H

#include <vector>
#include <boost/json/src.hpp>

namespace json = boost::json;

struct CreateMadelineInfo
{
    CreateMadelineInfo() {}

    CreateMadelineInfo(int inputNumbers, int layers, std::vector<int> neuronsPerLayer) : InputsNumber(inputNumbers),
                                                                                         Layers(layers),
                                                                                         NeuronsPerLayer(std::move(
                                                                                                 neuronsPerLayer)) {}

    int InputsNumber;
    int Layers;

    std::vector<int> NeuronsPerLayer;
};

CreateMadelineInfo tag_invoke(json::value_to_tag<CreateMadelineInfo>&, const json::value& value)
{
    int inputsNumber = value.at("InputsNumber").as_int64();
    int layers = value.at("Layers").as_int64();
    std::vector<int> neuronsPerLayer = json::value_to<std::vector<int>>(value.at("NeuronsPerLayer"));

    return CreateMadelineInfo(inputsNumber, layers, neuronsPerLayer);
}

void tag_invoke(const json::value_from_tag&, json::value& jv, CreateMadelineInfo const& info)
{
    // Assign a JSON value
    jv = {
            {"InputsNumber", info.InputsNumber}
    };
}

#endif //NEURONAL_NETWORKS_SERVER_MADELINE_H
