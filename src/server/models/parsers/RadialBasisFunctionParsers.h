//
// Created by cantte on 8/05/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_RADIALBASISFUNCTIONPARSERS_H
#define NEURONAL_NETWORKS_SERVER_RADIALBASISFUNCTIONPARSERS_H

struct RadialBasisFunctionTrainingInfo;
struct RadialBasisFunctionCreateInfo;

namespace boost
{
    namespace json
    {
        template<typename T>
        class value_to_tag;

        class value_from_tag;

        class value;
    }
}

RadialBasisFunctionCreateInfo
tag_invoke(const boost::json::value_to_tag<RadialBasisFunctionCreateInfo>&, const boost::json::value& value);

RadialBasisFunctionTrainingInfo
tag_invoke(const boost::json::value_to_tag<RadialBasisFunctionTrainingInfo>&, const boost::json::value& value);

#endif //NEURONAL_NETWORKS_SERVER_RADIALBASISFUNCTIONPARSERS_H
