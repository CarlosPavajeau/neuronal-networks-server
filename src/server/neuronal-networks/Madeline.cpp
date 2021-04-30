//
// Created by cantte on 28/04/21.
//

#include "Madeline.hpp"
#include "Session.hpp"

#include <algorithm>

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
    bool sw = false;
    int steps = 0;

    while (!sw && steps <= madelineTrainingInfo.MaxSteps)
    {
        ++steps;
        std::list<double> pattern_errors;
        for (int i = 0; i < madelineTrainingInfo.Inputs.size(); ++i)
        {
            const auto& input = madelineTrainingInfo.Inputs[i];
            const auto& expectedOutput = madelineTrainingInfo.Outputs[i];

            // Get output and calculate linear and pattern errors
            auto output = Output(input);

            std::vector<double> output_errors;
            for (int j = 0; j < expectedOutput.size(); ++j)
            {
                double error = output[j] - expectedOutput[j];
                output_errors.push_back(error);
            }

            double pattern_error = 0.0;
            std::for_each(output_errors.begin(), output_errors.end(),
                          [&pattern_error](double error) { pattern_error += std::abs(error); });
            pattern_errors.push_back(pattern_error);

            auto output_layer = _layers.back();
            for (int j = 0; j < output_errors.size(); ++j)
            {
                output_layer[j].SetError(output_errors[j]);
            }

            // Calculate layer errors
            for (int j = _layers.size() - 2; j >= 0; --j)
            {
                for (int k = 0; k < _layers[j].GetNeurons().size(); ++k)
                {
                    double error = 0.0;
                    for (int l = 0; l < _layers[j + 1].GetNeurons().size(); ++l)
                    {
                        const auto& neuron = _layers[j + 1][l];
                        error += neuron.GetError() * neuron.GetWeights()[k];
                    }

                    _layers[j][k].SetError(error);
                }
            }

            // Learn with delta rule
            for (int j = 0; j < _layers.size(); ++j)
            {
                for (auto& neuron : _layers[j].GetNeurons())
                {
                    neuron.Learn(j == 0 ? input : _layers[j - 1].GetOutputs(), neuron.GetError());
                }
            }

            // TODO: Temporary/permanent modification of weights
        }

        double iterationError = 0.0;
        for (const auto& patternError : pattern_errors)
        {
            iterationError += patternError;
        }
        iterationError /= pattern_errors.size();
        std::cout << "[server]: Iteration error " << iterationError << std::endl;
        sw = iterationError <= madelineTrainingInfo.ErrorTolerance;
    }

    return false;
}
