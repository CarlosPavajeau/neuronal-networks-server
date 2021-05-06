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

double SomNode::GetEuclideanDistance(const std::vector<double>& inputs) const
{
    assert(inputs.size() == _weights.size());

    double distance = 0.0;
    for (size_t i = 0; i < _weights.size(); ++i)
    {
        distance += std::pow((inputs[i] - _weights[i]), 2);
    }

    return std::sqrt(distance);
}

void SomNode::UpdateWeights(const std::vector<double>& inputs, double learning_rate, double winner_distance)
{
    for (int i = 0; i < _weights.size(); ++i)
    {
        _weights[i] += learning_rate * winner_distance * (inputs[i] - _weights[i]);
    }
}

double SomNode::GetDistance(const SomNode& node) const
{
    double diff_x = (double) GetX() - (double) node.GetX();
    double diff_y = (double) GetY() - (double) node.GetY();

    return std::pow(diff_x, 2) + std::pow(diff_y, 2);
}
