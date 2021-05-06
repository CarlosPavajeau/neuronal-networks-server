//
// Created by cantte on 4/05/21.
//

#include "SelfOrgMap.h"
#include "Session.hpp"

SelfOrgMap::SelfOrgMap(uint64 width, uint64 height, int num_inputs, double delta, bool tough_competition,
                       Session* session) : _session(session), _tough_competition(tough_competition), _delta(delta)
{
    _width = width;
    _height = height;
    _radius = (double) std::max(height, width) / 2.0;

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

bool SelfOrgMap::Train(const SelfOrgMapTrainingInfo& trainingInfo)
{
    double current_iteration_cost_function;
    double learning_rate = trainingInfo.LearningRate;

    for (int i = 0; i < trainingInfo.MaxSteps; ++i)
    {
        current_iteration_cost_function = 0.0;
        for (size_t j = 0; j < trainingInfo.Inputs.size(); ++j)
        {
            auto winner = GetBest(trainingInfo.Inputs[j]);
            current_iteration_cost_function += winner.second;

            if (_tough_competition)
            {
                _som[winner.first.X][winner.first.Y].UpdateWeights(trainingInfo.Inputs[j], learning_rate,
                                                                   winner.second);

            } else
            {
                double neighborhood_radius = _radius * std::exp(-i / (trainingInfo.MaxSteps / std::log(_radius)));
                double neighborhood_diameter = neighborhood_radius * 2;
                double neighborhood_squared = std::pow(neighborhood_radius, 2);

                int start_x = (int) std::max(0.0, (double) winner.first.X - neighborhood_radius - 1);
                int start_y = (int) std::max(0.0, (double) winner.first.Y - neighborhood_radius - 1);
                int end_x = (int) std::min((double) _width, start_x + neighborhood_diameter + 1);
                int end_y = (int) std::min((double) _height, start_y + neighborhood_diameter + 1);

                for (int x = start_x; x < end_x; ++x)
                {
                    for (int y = start_y; y < end_y; ++y)
                    {
                        double distance = _som[x][y].GetDistance(_som[winner.first.X][winner.first.Y]);
                        if (distance <= neighborhood_squared)
                        {
                            double influence = std::exp(-distance / (2 * neighborhood_squared));
                            _som[x][y].UpdateWeights(trainingInfo.Inputs[j], learning_rate, influence);
                        }
                    }
                }
            }
        }

        learning_rate = trainingInfo.LearningRate * exp(-(double) (i + 1) / trainingInfo.MaxSteps);

        /*if (_session)
        {
            if ((i % (trainingInfo.MaxSteps / 10)) == 0)
            {
                _session->WriteIterationError(current_iteration_cost_function);
            }
        }*/

        current_iteration_cost_function /= trainingInfo.Inputs.size();
        std::cout << current_iteration_cost_function << std::endl;
        if (current_iteration_cost_function <= trainingInfo.ErrorTolerance)
        {
            return true;
        }
    }

    return false;
}

void SelfOrgMap::GetOutput(const std::vector<double>& input, std::vector<double>* output) const
{
    std::vector<std::vector<double>> euclidean_distances;
    euclidean_distances.resize(_som.size());
    int minX, minY;
    double winner_dist = 999;

    for (int k = 0; k < _som.size(); ++k)
    {
        euclidean_distances[k].resize(_som[k].size());
        for (int l = 0; l < _som[k].size(); ++l)
        {
            euclidean_distances[k][l] = _som[k][l].GetEuclideanDistance(input);
            if (euclidean_distances[k][l] < winner_dist)
            {
                winner_dist = euclidean_distances[k][l];
                minX = k;
                minY = l;
            }
        }
    }

    output->resize(_som[minX][minY].GetWeightsVectorSize());
    for (int i = 0; i < _som[minX][minY].GetWeightsVectorSize(); ++i)
    {
        (*output)[i] = _som[minX][minY].GetWeights()[i];
    }
}

BestValue SelfOrgMap::GetBest(const std::vector<double>& input) const
{
    Point point;
    double winner_dist = std::numeric_limits<double>::max();

    for (int i = 0; i < _som.size(); ++i)
    {
        for (int j = 0; j < _som[i].size(); ++j)
        {
            double euclidean_distance = _som[i][j].GetEuclideanDistance(input);
            if (euclidean_distance < winner_dist)
            {
                winner_dist = euclidean_distance;
                point.X = i;
                point.Y = j;
            }
        }
    }

    return {point, winner_dist};
}
