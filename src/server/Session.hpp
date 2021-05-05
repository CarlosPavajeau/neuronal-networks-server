//
// Created by cantte on 26/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_SESSION_HPP
#define NEURONAL_NETWORKS_SERVER_SESSION_HPP

#include "Message.hpp"

#include <memory>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class MultiLayerPerceptron;

class SelfOrgMap;

class Session : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(tcp::socket socket);

    ~Session();

    void Start();

    void Write(const Message& msg);

    void WriteIterationError(double iteration_error);

    Message ReadMessage() const;

    void HandleServerSide(Message& message);

    void HandleInitMadeline(Message& message);

    void HandleStartTrainingMadeline(Message& message);

    void HandleSimulateDataMadeline(Message& message);

    void HandleInitSelfOrgMap(Message& message);

    void HandleStartTrainingSelfOrgMap(Message& message);

    void HandleSimulateDataSelfOrgMap(Message& message);

private:
    void DoReadHeader();

    void DoReadBody();

    void OnInitMadeline();
    void OnInitSelfOrgMap();

    tcp::socket _socket;
    Message _read_msg, _write_msg;
    MultiLayerPerceptron* _madeline;
    SelfOrgMap* _self_org_map;
};

#endif //NEURONAL_NETWORKS_SERVER_SESSION_HPP
