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

class Session : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(tcp::socket socket) : _socket(std::move(socket)) {}

    void Start();

    void Write(const Message& msg);

    Message ReadMessage() const;

    void HandleServerSide(Message& message);

    void HandleInitNeuron(Message& message);

private:
    void DoReadHeader();

    void DoReadBody();

    tcp::socket _socket;
    Message _read_msg, _write_msg;
};

#endif //NEURONAL_NETWORKS_SERVER_SESSION_HPP
