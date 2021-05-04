//
// Created by cantte on 27/04/21.
//

#include "Session.hpp"
#include "Message.hpp"
#include "MultiLayerPerceptron.hpp"
#include "Opcodes.hpp"
#include "OpcodeTable.h"
#include "MadelineParsers.h"

#include <boost/json/src.hpp>

namespace json = boost::json;

Session::Session(tcp::socket socket) : _socket(std::move(socket)), _madeline(nullptr)
{

}

Session::~Session()
{
    if (!_madeline)
    {
        return;
    }

    delete _madeline;
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
                                    } else
                                    {
                                        std::cout << "[server]: A handler has not been defined for the opcode: "
                                                  << GetOpcodeNameForLogging(opcode) << std::endl;
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

    _madeline = new MultiLayerPerceptron(madelineCreateInfo.NeuronsPerLayer, madelineCreateInfo.ActivationFunctions, this);

    Message responseMessage;
    responseMessage.Opcode(SMSG_INIT_MADELINE);

    const char* r_message = "Init neuron success!";

    responseMessage.BodyLength(std::strlen(r_message));
    std::memcpy(responseMessage.Body(), r_message, responseMessage.BodyLength());

    responseMessage.EncodeHeader();

    Write(responseMessage);
}

void Session::HandleStartTrainingMadeline(Message& message)
{
    json::parser parser;
    parser.write(message.Body(), message.BodyLength());
    json::value value = parser.release();

    const MadelineTrainingInfo madelineTrainingInfo = json::value_to<MadelineTrainingInfo>(value);

    if (_madeline)
    {
        if (_madeline->Train(madelineTrainingInfo))
        {
            std::cout << "[server]: Training success!" << std::endl;
        }
    } else
    {
        std::cout << "[server]: MultiLayerPerceptron not init" << std::endl;
    }
}

void Session::HandleSimulateData(Message& message)
{
    json::parser parser;
    parser.write(message.Body(), message.BodyLength());
    json::value value = parser.release();

    const std::vector<double> inputs = json::value_to<std::vector<double>>(value);
    std::vector<double> outputs;

    if (_madeline)
    {
        _madeline->GetOutput(inputs, &outputs);

        Message responseMessage;
        responseMessage.Opcode(SMSG_SIMULATE_DATA);

        json::value responseValue = json::value_from(outputs);
        std::string response_str = json::serialize(responseValue);

        const char* r_message = response_str.c_str();

        responseMessage.BodyLength(std::strlen(r_message));
        std::memcpy(responseMessage.Body(), r_message, responseMessage.BodyLength());

        responseMessage.EncodeHeader();

        Write(responseMessage);
    } else
    {
        std::cout << "[server]: MultiLayerPerceptron not init!";
    }
}
