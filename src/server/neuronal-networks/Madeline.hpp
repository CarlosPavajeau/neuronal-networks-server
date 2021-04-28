//
// Created by cantte on 28/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_MADELINEMODELS_H
#define NEURONAL_NETWORKS_SERVER_MADELINE_H

#include "Layer.hpp"
#include "AllModels.h"

#include <vector>

class Session;

class Madeline
{
    friend class Session;

public:
    explicit Madeline(const MadelineCreateInfo& madelineCreateInfo, Session* session);

    std::vector<double> Output(const std::vector<double>& inputs);

    bool Train(const MadelineTrainingInfo& madelineTrainingInfo);

private:
    std::vector<Layer> _layers;
    Session* _session;
};


#endif //NEURONAL_NETWORKS_SERVER_MADELINEMODELS_H
