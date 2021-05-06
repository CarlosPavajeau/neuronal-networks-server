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

typedef std::pair<Point, double> BestValue;

class SelfOrgMap
{
public:
    explicit SelfOrgMap(uint64 width, uint64 height, int num_inputs, double delta, bool tough_competition,
                        Session* session);

    bool Train(const SelfOrgMapTrainingInfo& trainingInfo);

    void GetOutput(const std::vector<double>& input, std::vector<double>* output) const;

    BestValue GetBest(const std::vector<double>& input) const;

    std::vector<std::vector<SomNode>> GetMap() const { return _som; }

protected:
    std::vector<std::vector<SomNode>> _som;
    Session* _session;
    double _delta;
    uint64 _width, _height;
    double _radius;
    bool _tough_competition;
};

#endif //NEURONAL_NETWORKS_SERVER_SELFORGMAP_H
