//
// Created by cantte on 26/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_SESSION_HPP
#define NEURONAL_NETWORKS_SERVER_SESSION_HPP

#include "Message.hpp"
#include "Opcodes.hpp"

#include <memory>
#include <iostream>
#include <deque>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class MultiLayerPerceptron;

class SelfOrgMap;

class RadialBasisFunction;

typedef std::deque<Message> MessageQueue;

class Session : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(tcp::socket socket);

    ~Session();

    void Start();

    void Write(const Message& msg);

    void WriteIterationError(Opcodes opcode, double iteration_error);

    Message ReadMessage() const;

    void HandleServerSide(Message& message);

    void HandleInitMadeline(Message& message);

    void HandleStartTrainingMadeline(Message& message);

    void HandleSimulateDataMadeline(Message& message);

    void HandleInitSelfOrgMap(Message& message);

    void HandleStartTrainingSelfOrgMap(Message& message);

    void HandleSimulateDataSelfOrgMap(Message& message);

    void HandleInitRbf(Message& message);

    void HandleStartTrainingRbf(Message& message);

    void HandleSimulateDataRbf(Message& message);

private:
    void DoReadHeader();

    void DoReadBody();

    void DoWrite();

    void OnInitMadeline();

    void OnInitSelfOrgMap();

    void OnInitRbf();
    
    tcp::socket _socket;
    Message _read_msg, _write_msg;
    MessageQueue _write_msgs;
    MultiLayerPerceptron* _madeline;
    SelfOrgMap* _self_org_map;
    RadialBasisFunction* _radial_basis_function;
};

#endif //NEURONAL_NETWORKS_SERVER_SESSION_HPP
