//
// Created by cantte on 27/04/21.
//

#include "Session.hpp"
#include "Message.hpp"
#include "Opcodes.hpp"
#include "OpcodeTable.h"

void Session::Start()
{
    DoReadHeader();
}

void Session::Write(const Message& msg)
{
    _write_msg = msg;
    auto self(shared_from_this());

    boost::asio::async_write(_socket,
                             boost::asio::buffer(_write_msg.Data(), _write_msg.Length()),
                             [this, self](boost::system::error_code ec, std::size_t /*length*/)
                             {
                                 if (!ec)
                                 {
                                     OpcodeClient opcode = static_cast<OpcodeClient>(_write_msg.Opcode());
                                     std::cout << "[server]: Send opcode: " << GetOpcodeNameForLogging(opcode)
                                               << std::endl;
                                 } else
                                 {
                                     std::cout << "Error: " << ec.message() << std::endl;
                                 }
                             });
}

Message Session::ReadMessage() const
{
    return _read_msg;
}

void Session::DoReadHeader()
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

void Session::DoReadBody()
{
    auto self(shared_from_this());

    boost::asio::async_read(_socket,
                            boost::asio::buffer(_read_msg.Body(), _read_msg.BodyLength()),
                            [this, self](boost::system::error_code ec, std::size_t /*length*/)
                            {
                                if (!ec)
                                {
                                    OpcodeClient opcode = static_cast<OpcodeClient>(ReadMessage().Opcode());
                                    std::cout << "[server]: Receive opcode: " << GetOpcodeNameForLogging(opcode)
                                              << std::endl;

                                    if (ClientOpcodeHandler const* opHandle = opcodeTable[opcode])
                                    {
                                        opHandle->Call(this, _read_msg);
                                    }

                                    DoReadHeader();
                                } else
                                {
                                    std::cout << "[server]: Error: Can't read body" << std::endl;
                                }
                            });
}

void Session::HandleInitNeuron(Message& message)
{
    Message responseMessage;
    responseMessage.Opcode(MSGS_INIT_NEURON);

    const char* r_message = "Init neuron";

    responseMessage.BodyLength(std::strlen(r_message));
    std::memcpy(responseMessage.Body(), r_message, responseMessage.BodyLength());

    responseMessage.EncodeHeader();

    Write(responseMessage);
}

