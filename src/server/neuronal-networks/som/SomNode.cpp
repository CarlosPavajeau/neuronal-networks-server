//
// Created by cantte on 4/05/21.
//

#include "SomNode.h"
#include <cassert>

SomNode::SomNode(const Point& coordinate, int num_inputs) : Node(num_inputs)
{
    _coordinate = coordinate;
}

SomNode::SomNode(uint64 x, uint64 y, int num_inputs) : Node(num_inputs)
{
    _coordinate = Point(x, y);
}

SomNode::SomNode()
{
    _coordinate = Point(0, 0);
}

void SomNode::SetCoordinate(uint64 x, uint64 y)
{
    _coordinate = Point(x, y);
}

double SomNode::GetEuclideanDistance(std::vector<double> inputs) const
{
    assert(inputs.size() == _weights.size());

    double distance = 0.0;
    for (size_t i = 0; i < _weights.size(); ++i)
    {
        distance += std::pow((inputs[i] - _weights[i]), 2);
    }

    return std::sqrt(distance);
}

void SomNode::UpdateWeights(double learning_rate, double winner_distance)
{
    for (double& _weight : _weights)
    {
        _weight += learning_rate * winner_distance;
    }
}
