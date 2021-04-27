//
// Created by cantte on 26/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_SERVER_HPP
#define NEURONAL_NETWORKS_SERVER_SERVER_HPP

#include "Message.hpp"
#include "Session.hpp"

#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Server
{
public:
    Server(boost::asio::io_context& io_context, const tcp::endpoint& endpoint) : _aceptor(io_context, endpoint)
    {
        DoAccept();
    }

private:
    void DoAccept()
    {
        _aceptor.async_accept(
                [this](boost::system::error_code ec, tcp::socket socket)
                {
                    if (!ec)
                    {
                        std::make_shared<Session>(std::move(socket))->Start();
                    }

                    DoAccept();
                });
    }

    tcp::acceptor _aceptor;
};

#endif //NEURONAL_NETWORKS_SERVER_SERVER_HPP
