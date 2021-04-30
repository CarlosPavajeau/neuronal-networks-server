//
// Created by cantte on 29/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_MADELINEPARSERS_H
#define NEURONAL_NETWORKS_SERVER_MADELINEPARSERS_H

struct MadelineCreateInfo;

class Madeline;

class Layer;

class Neuron;

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

MadelineCreateInfo tag_invoke(const boost::json::value_to_tag<MadelineCreateInfo>&, const boost::json::value& value);

void tag_invoke(const boost::json::value_from_tag&, boost::json::value& jv, Madeline const& madeline);

void tag_invoke(const boost::json::value_from_tag&, boost::json::value& jv, Layer const& layer);

void tag_invoke(const boost::json::value_from_tag&, boost::json::value& jv, Neuron const& neuron);

#endif //NEURONAL_NETWORKS_SERVER_MADELINEPARSERS_H
