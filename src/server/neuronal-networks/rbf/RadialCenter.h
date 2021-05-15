//
// Created by cantte on 7/05/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_RADIALCENTER_H
#define NEURONAL_NETWORKS_SERVER_RADIALCENTER_H

#include "Define.hpp"
#include <vector>

class RadialCenter
{
public:
    explicit RadialCenter(int num_inputs);

    void GetEuclideanDistance(const std::vector<double>& inputs, double* output) const;

    void SetCentroids(const std::vector<double>& centroids);
    void CentroidsInitialization(double min, double max);

private:
    size_t _num_inputs;

    std::vector<double> _centroids;
};


#endif //NEURONAL_NETWORKS_SERVER_RADIALCENTER_H
