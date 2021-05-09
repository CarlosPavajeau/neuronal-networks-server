//
// Created by cantte on 8/05/21.
//
#include "RadialBasisFunctionParsers.h"
#include "Define.hpp"
#include "RadialBasisFunctionModels.h"

#include <boost/json.hpp>

namespace json = boost::json;

RadialBasisFunctionCreateInfo
tag_invoke(const boost::json::value_to_tag<RadialBasisFunctionCreateInfo>&, const boost::json::value& value)
{
    uint64 inputs_number = value.at("InputNumbers").as_int64();
    uint64 radial_centers = value.at("RadialCenters").as_int64();

    return RadialBasisFunctionCreateInfo(inputs_number, radial_centers);
}

RadialBasisFunctionTrainingInfo
tag_invoke(const boost::json::value_to_tag<RadialBasisFunctionTrainingInfo>&, const boost::json::value& value)
{
    double error_tolerance = value.at("ErrorTolerance").as_double();
    int max_steps = (int) value.at("MaxSteps").as_int64();

    auto inputs = json::value_to<std::vector<std::vector<double>>>(value.at("Inputs"));
    auto outputs = json::value_to<std::vector<double>>(value.at("Outputs"));

    return RadialBasisFunctionTrainingInfo(inputs, outputs, error_tolerance, max_steps);
}
