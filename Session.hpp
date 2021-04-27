//
// Created by cantte on 26/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_SESSION_HPP
#define NEURONAL_NETWORKS_SERVER_SESSION_HPP

#include "Message.hpp"
#include "Opcodes.hpp"

#include <memory>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session>
{
public:
    explicit Session(tcp::socket socket) : _socket(std::move(socket)) {}

    void Start()
    {
        DoReadHeader();
    }

    void Write(const Message& msg)
    {
        _write_msg = msg;
        auto self(shared_from_this());

        boost::asio::async_write(_socket,
                                 boost::asio::buffer(_write_msg.Data(), _write_msg.Length()),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                 {
                                     if (!ec)
                                     {
                                         std::cout << "[server]: Send Opcode: "
                                                   << GetOpCodeName(static_cast<Opcodes>(_write_msg.Opcode()))
                                                   << std::endl;
                                     } else
                                     {
                                         std::cout << "Error: " << ec.message() << std::endl;
                                     }
                                 });
    }

    Message ReadMessage() const
    {
        return _read_msg;
    }

private:
    void DoReadHeader()
    {
        auto self(shared_from_this());

        boost::asio::async_read(_socket,
                                boost::asio::buffer(_read_msg.Data(), Message::header_length),
                                [this, self](boost::system::error_code ec, std::size_t /*Length*/)
                                {
                                    if (!ec && _read_msg.DecodeHeader())
                                    {
                                        DoReadBody();
                                    } else
                                    {
                                        std::cout << "[server]: Error: Client disconnect!" << std::endl;
                                    }
                                });
    }

    void DoReadBody()
    {
        auto self(shared_from_this());

        boost::asio::async_read(_socket,
                                boost::asio::buffer(_read_msg.Body(), _read_msg.BodyLength()),
                                [this, self](boost::system::error_code ec, std::size_t /*length*/)
                                {
                                    if (!ec)
                                    {
                                        // TODO: Handle op codes and data
                                        std::cout << "[server]: Receive Opcode: "
                                                  << GetOpCodeName(static_cast<Opcodes>(_read_msg.Opcode()))
                                                  << std::endl;

                                        if (ReadMessage().Opcode() == MSGC_INIT_NEURON)
                                        {
                                            Message responseMessage = Message();
                                            responseMessage.Opcode(MSGS_INIT_NEURON);

                                            const char* message = "Init neuron";
                                            responseMessage.BodyLength(std::strlen(message));
                                            std::memcpy(responseMessage.Body(), message, responseMessage.BodyLength());

                                            responseMessage.EncodeHeader();

                                            Write(responseMessage);
                                        }

                                        DoReadHeader();
                                    } else
                                    {
                                        std::cout << "[server]: Error: Can't read body" << std::endl;
                                    }
                                });
    }

    tcp::socket _socket;
    Message _read_msg, _write_msg;
};

#endif //NEURONAL_NETWORKS_SERVER_SESSION_HPP
