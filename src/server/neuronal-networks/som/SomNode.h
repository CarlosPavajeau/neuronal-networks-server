//
// Created by cantte on 4/05/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_SOMNODE_H
#define NEURONAL_NETWORKS_SERVER_SOMNODE_H

#include "Node.h"
#include "Utils.h"

class SomNode : public Node
{
public:
    explicit SomNode(const Point& coordinate, int num_inputs);

    SomNode(uint64 x, uint64 y, int num_inputs);

    SomNode();

    const Point& GetCoordinate() const { return _coordinate; }

    void SetCoordinate(const Point& new_coordinate) { _coordinate = new_coordinate; }

    void SetCoordinate(uint64 x, uint64 y);

    uint64 GetX() const { return _coordinate.X; }

    uint64 GetY() const { return _coordinate.Y; }

    void SetX(uint64 new_x) { _coordinate.X = new_x; }

    void SetY(uint64 new_y) { _coordinate.X = new_y; }

    double GetEuclideanDistance(std::vector<double> inputs) const;

    void UpdateWeights(double learning_rate, double winner_distance);

protected:
    Point _coordinate;
    std::vector<float> _distance_to;
};


#endif //NEURONAL_NETWORKS_SERVER_SOMNODE_H
