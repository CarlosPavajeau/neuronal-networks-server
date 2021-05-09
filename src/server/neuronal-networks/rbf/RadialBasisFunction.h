//
// Created by cantte on 7/05/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_RADIALBASISFUNCTION_H
#define NEURONAL_NETWORKS_SERVER_RADIALBASISFUNCTION_H

#include "Define.hpp"
#include "Node.h"
#include "RadialBasisFunctionModels.h"
#include "RadialCenter.h"

#include <vector>

class Session;

class RadialBasisFunction
{
public:
    RadialBasisFunction(uint64 num_inputs, uint64 radial_centers, Session* session);

    void GetOutput(const std::vector<double>& input, double* output) const;

    bool Train(const RadialBasisFunctionTrainingInfo& trainingInfo);

private:
    void SetUpRadialCenters(const std::vector<std::vector<double>>& inputs);

    size_t _num_inputs;

    std::vector<RadialCenter> _radial_centers;
    Node _output_node;

    Session* _session;
};


#endif //NEURONAL_NETWORKS_SERVER_RADIALBASISFUNCTION_H
