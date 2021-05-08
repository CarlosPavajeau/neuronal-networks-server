//
// Created by cantte on 7/05/21.
//

#include "RadialCenter.h"
#include "Utils.h"
#include "Random.h"
#include <cassert>

RadialCenter::RadialCenter(int num_inputs)
{
    _num_inputs = num_inputs;
    _centroids.resize(num_inputs, 0);
}

void RadialCenter::GetEuclideanDistance(const std::vector<double>& inputs, double* output) const
{
    assert(_num_inputs == inputs.size());

    double distance = 0.0;
    for (int i = 0; i < _num_inputs; ++i)
    {
        distance += std::pow(inputs[i] - _centroids[i], 2);
    }

    *output = thin_plate_spline(distance);
}

void RadialCenter::SetCentroids(const std::vector<double>& centroids)
{
    assert(centroids.size() == _num_inputs);

    _centroids = centroids;
}

void RadialCenter::CentroidsInitialization(double min, double max)
{
    std::generate_n(_centroids.begin(), _num_inputs, gen_rand(min, max));
}
