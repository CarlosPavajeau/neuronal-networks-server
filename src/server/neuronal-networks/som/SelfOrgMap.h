//
// Created by cantte on 4/05/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_SELFORGMAP_H
#define NEURONAL_NETWORKS_SERVER_SELFORGMAP_H

#include "Define.hpp"
#include "SomNode.h"
#include "SelfOrgMapModels.h"

#include <vector>

class Session;

class SelfOrgMap
{
public:
    explicit SelfOrgMap(uint64 width, uint64 height, int num_inputs, double delta, bool tough_competition,
                        Session* session);

    bool Train(const SelfOrgMapTrainingInfo& madelineTrainingInfo);

protected:
    std::vector<std::vector<SomNode>> _som;
    Session* _session;
    double _delta;
    bool _tough_competition;
};

#endif //NEURONAL_NETWORKS_SERVER_SELFORGMAP_H
