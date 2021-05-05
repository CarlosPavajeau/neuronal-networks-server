//
// Created by cantte on 5/05/21.
//

#include "SelfOrgMapParsers.h"
#include "SelfOrgMapModels.h"

#include <vector>
#include <boost/json.hpp>

namespace json = boost::json;

SelfOrgMapCreateInfo
tag_invoke(const boost::json::value_to_tag<SelfOrgMapCreateInfo>&, const boost::json::value& value)
{
    uint64 width = value.at("Width").as_uint64();
    uint64 height = value.at("Height").as_uint64();
    int num_inputs = value.at("InputsNumber").as_uint64();
    double delta = value.at("Delta").as_double();
    bool tough_competition = value.at("ToughCompetition").as_bool();

    return {width, height, num_inputs, delta, tough_competition};
}

SelfOrgMapTrainingInfo
tag_invoke(const boost::json::value_to_tag<SelfOrgMapTrainingInfo>&, const boost::json::value& value)
{
    double error_tolerance = value.at("ErrorTolerance").as_double();
    double learning_rate = value.at("LearningRate").as_double();
    int max_steps = value.at("MaxSteps").as_int64();
    auto inputs = json::value_to<std::vector<std::vector<double>>>(value.at("Inputs"));

    return SelfOrgMapTrainingInfo(inputs, error_tolerance, learning_rate, max_steps);
}
