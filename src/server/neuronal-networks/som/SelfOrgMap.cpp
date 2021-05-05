//
// Created by cantte on 4/05/21.
//

#include "SelfOrgMap.h"
#include "Session.hpp"

SelfOrgMap::SelfOrgMap(uint64 width, uint64 height, int num_inputs, double delta, bool tough_competition,
                       Session* session) : _session(session), _tough_competition(tough_competition), _delta(delta)
{
    for (int x = 0; x < width; ++x)
    {
        std::vector<SomNode> nodes;
        for (int y = 0; y < height; ++y)
        {
            SomNode node(x, y, num_inputs);
            nodes.push_back(node);
        }
        _som.push_back(nodes);
    }
}

bool SelfOrgMap::Train(const MadelineTrainingInfo& madelineTrainingInfo)
{
    double current_iteration_cost_function;
    double learning_rate = madelineTrainingInfo.LearningRate;

    for (int i = 0; i < madelineTrainingInfo.MaxSteps; ++i)
    {
        if (i > 1)
        {
            learning_rate /= i;
        }

        current_iteration_cost_function = 0.0;
        for (size_t j = 0; j < madelineTrainingInfo.Inputs.size(); ++j)
        {
            std::vector<std::vector<double>> euclidean_distances;
            euclidean_distances.resize(_som.size());
            int minX, minY;
            double winner_dist = 999;
            for (int k = 0; k < _som.size(); ++k)
            {
                euclidean_distances.resize(_som[k].size());
                for (int l = 0; l < _som[k].size(); ++l)
                {
                    euclidean_distances[k][l] = _som[k][l].GetEuclideanDistance(madelineTrainingInfo.Inputs[j]);
                    if (euclidean_distances[k][l] < winner_dist)
                    {
                        winner_dist = euclidean_distances[k][l];
                        minX = k;
                        minY = l;
                    }
                }
            }

            current_iteration_cost_function += winner_dist;

            if (_tough_competition)
            {
                _som[minX][minY].UpdateWeights(learning_rate, winner_dist);
            } else
            {
                double min_distance = winner_dist + _delta;
                for (int k = 0; k < euclidean_distances.size(); ++k)
                {
                    for (int l = 0; l < euclidean_distances[k].size(); ++l)
                    {
                        if (euclidean_distances[k][l] < min_distance)
                        {
                            _som[k][l].UpdateWeights(learning_rate, winner_dist);
                        }
                    }
                }
            }
        }

        current_iteration_cost_function /= madelineTrainingInfo.Inputs.size();

        if (_session)
        {
            if ((i % (madelineTrainingInfo.MaxSteps / 10)) == 0)
            {
                _session->WriteIterationError(current_iteration_cost_function);
            }
        }

        if (current_iteration_cost_function <= madelineTrainingInfo.ErrorTolerance)
        {
            return true;
        }
    }

    return false;
}
