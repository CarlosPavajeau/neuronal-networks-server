//
// Created by cantte on 27/04/21.
//

#include "Session.hpp"
#include "Message.hpp"
#include "Madeline.hpp"
#include "Opcodes.hpp"
#include "OpcodeTable.h"
#include "MadelineParsers.h"

#include <boost/json/src.hpp>

namespace json = boost::json;

Session::Session(tcp::socket socket) : _socket(std::move(socket)), _madeline(nullptr)
{

}

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

void Session::HandleServerSide(Message& message)
{
    std::cout << "[server]: Received server-side opcode "
              << GetOpcodeNameForLogging(static_cast<OpcodeClient>(message.Opcode()));
}

void Session::HandleInitMadeline(Message& message)
{
    json::parser parser;
    parser.write(message.Body(), message.BodyLength());
    json::value value = parser.release();

    const MadelineCreateInfo madelineCreateInfo = json::value_to<MadelineCreateInfo>(value);

    _madeline = new Madeline(madelineCreateInfo, this);

    Message responseMessage;
    responseMessage.Opcode(SMSG_INIT_MADELINE);

    json::value responseValue = json::value_from(*_madeline);
    std::string responseStr = json::serialize(responseValue);
    const char* r_message = responseStr.c_str();

    responseMessage.BodyLength(std::strlen(r_message));
    std::memcpy(responseMessage.Body(), r_message, responseMessage.BodyLength());

    responseMessage.EncodeHeader();

    Write(responseMessage);
}
