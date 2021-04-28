//
// Created by cantte on 28/04/21.
//

#include "Madeline.hpp"
#include "Session.hpp"

Madeline::Madeline(const MadelineCreateInfo& madelineCreateInfo, Session* session) : _session(session)
{
    for (int i = 0; i < madelineCreateInfo.NeuronsPerLayer.size(); ++i)
    {
        LayerCreateInfo layerCreateInfo{
                .InputsNumber = i == 0 ? madelineCreateInfo.InputNumbers
                                       : madelineCreateInfo.NeuronsPerLayer[i - 1],
                .NeuronsNumber = madelineCreateInfo.NeuronsPerLayer[i],
                .Trigger = madelineCreateInfo.Triggers[i],
                .TrainingRate = madelineCreateInfo.TrainingRate};
        Layer layer(layerCreateInfo);
        _layers.push_back(layer);
    }
}

std::vector<double> Madeline::Output(const std::vector<double>& inputs)
{
    std::vector<double> outputs = inputs;
    for (auto& layer : _layers)
    {
        outputs = layer.Output(outputs);
    }
    return outputs;
}

bool Madeline::Train(const MadelineTrainingInfo& madelineTrainingInfo)
{
    return false;
}
