//
// Created by cantte on 5/05/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_SELFORGMAPPARSERS_H
#define NEURONAL_NETWORKS_SERVER_SELFORGMAPPARSERS_H

struct SelfOrgMapCreateInfo;
struct SelfOrgMapTrainingInfo;
struct Point;

class SelfOrgMap;

class SomNode;

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

SelfOrgMapCreateInfo
tag_invoke(const boost::json::value_to_tag<SelfOrgMapCreateInfo>&, const boost::json::value& value);

SelfOrgMapTrainingInfo
tag_invoke(const boost::json::value_to_tag<SelfOrgMapTrainingInfo>&, const boost::json::value& value);

void tag_invoke(const boost::json::value_from_tag&, boost::json::value& jv, SelfOrgMap const& selfOrgMap);

void tag_invoke(const boost::json::value_from_tag&, boost::json::value& jv, SomNode const& somNode);
void tag_invoke(const boost::json::value_from_tag&, boost::json::value& jv, Point const& point);

#endif //NEURONAL_NETWORKS_SERVER_SELFORGMAPPARSERS_H
