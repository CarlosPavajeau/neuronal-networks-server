//
// Created by cantte on 27/04/21.
//

#include "Session.hpp"
#include "Message.hpp"
#include "MultiLayerPerceptron.hpp"
#include "SelfOrgMap.h"
#include "RadialBasisFunction.h"
#include "Opcodes.hpp"
#include "OpcodeTable.h"
#include "MadelineParsers.h"
#include "SelfOrgMapParsers.h"
#include "RadialBasisFunctionParsers.h"

#include <boost/json/src.hpp>

namespace json = boost::json;

Session::Session(tcp::socket socket) : _socket(std::move(socket)), _madeline(nullptr), _self_org_map(nullptr),
                                       _radial_basis_function(nullptr)
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
                                     auto opcode = static_cast<OpcodeClient>(_write_msg.Opcode());
                                     std::cout << "[server]: Send opcode: " << GetOpcodeNameForLogging(opcode)
                                               << std::endl;
                                 } else
                                 {
                                     std::cout << "Error: " << ec.message() << std::endl;
                                 }
                             });
}

void Session::WriteIterationError(Opcodes opcode, double iteration_error)
{
    Message response_message;
    response_message.Opcode(opcode);

    std::string message_str = std::to_string(iteration_error);
    const char* r_message = message_str.c_str();

    response_message.BodyLength(std::strlen(r_message));
    std::memcpy(response_message.Body(), r_message, response_message.BodyLength());
    response_message.EncodeHeader();

    Write(response_message);
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

    _madeline = new MultiLayerPerceptron(madelineCreateInfo.NeuronsPerLayer, madelineCreateInfo.ActivationFunctions,
                                         this);

    OnInitMadeline();
}

void Session::HandleStartTrainingMadeline(Message& message)
{
    json::parser parser;
    parser.write(message.Body(), message.BodyLength());
    json::value value = parser.release();

    const MadelineTrainingInfo madelineTrainingInfo = json::value_to<MadelineTrainingInfo>(value);

    if (_madeline)
    {
        Message trainMessage;
        trainMessage.Opcode(SMSG_TRAIN_MADELINE);
        const char* r_message;
        if (_madeline->Train(madelineTrainingInfo))
        {
            std::cout << "[server]: Training success!" << std::endl;
            r_message = "true";
        } else
        {
            r_message = "false";
        }

        trainMessage.BodyLength(std::strlen(r_message));
        std::memcpy(trainMessage.Body(), r_message, trainMessage.BodyLength());
        trainMessage.EncodeHeader();

        Write(trainMessage);
    } else
    {
        std::cout << "[server]: MultiLayerPerceptron not init" << std::endl;
    }
}

void Session::HandleSimulateDataMadeline(Message& message)
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
        responseMessage.Opcode(SMSG_SIMULATE_DATA_MADELINE);

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

void Session::OnInitMadeline()
{
    Message response_message;
    response_message.Opcode(SMSG_INIT_MADELINE);

    json::value value_to_send = json::value_from(*_madeline);
    std::string message_str = json::serialize(value_to_send);

    const char* r_message = message_str.c_str();
    response_message.BodyLength(std::strlen(r_message));
    std::memcpy(response_message.Body(), r_message, response_message.BodyLength());
    response_message.EncodeHeader();

    Write(response_message);
}

void Session::HandleInitSelfOrgMap(Message& message)
{
    json::parser parser;
    parser.write(message.Body(), message.BodyLength());
    json::value value = parser.release();

    const SelfOrgMapCreateInfo createInfo = json::value_to<SelfOrgMapCreateInfo>(value);

    _self_org_map = new SelfOrgMap(createInfo.Width, createInfo.Height, createInfo.InputNumbers, createInfo.Delta,
                                   createInfo.ToughCompetition, this);

    OnInitSelfOrgMap();
}

void Session::HandleStartTrainingSelfOrgMap(Message& message)
{
    if (_self_org_map)
    {
        json::parser parser;
        parser.write(message.Body(), message.BodyLength());
        json::value value = parser.release();

        const SelfOrgMapTrainingInfo trainingInfo = json::value_to<SelfOrgMapTrainingInfo>(value);

        Message trainMessage;
        trainMessage.Opcode(SMSG_TRAIN_SELF_ORG_MAP);
        const char* r_message;
        if (_self_org_map->Train(trainingInfo))
        {
            std::cout << "[server]: Training success!" << std::endl;
            r_message = "true";
        } else
        {
            std::cout << "[server]: Not training!" << std::endl;
            r_message = "false";
        }

        trainMessage.BodyLength(std::strlen(r_message));
        std::memcpy(trainMessage.Body(), r_message, trainMessage.BodyLength());
        trainMessage.EncodeHeader();

        Write(trainMessage);
    } else
    {
        std::cout << "[server]: SelfOrgMap not init!" << std::endl;
    }
}

void Session::HandleSimulateDataSelfOrgMap(Message& message)
{
    json::parser parser;
    parser.write(message.Body(), message.BodyLength());
    json::value value = parser.release();

    const std::vector<double> inputs = json::value_to<std::vector<double>>(value);
    std::vector<double> outputs;

    if (_self_org_map)
    {
        _self_org_map->GetOutput(inputs, &outputs);

        Message responseMessage;
        responseMessage.Opcode(SMSG_SIMULATE_DATA_SELF_ORG_MAP);

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

void Session::OnInitSelfOrgMap()
{
    Message response_message;
    response_message.Opcode(SMSG_INIT_SELF_ORG_MAP);

    json::value value_to_send = json::value_from(*_self_org_map);
    std::string response_str = json::serialize(value_to_send);

    const char* r_message = response_str.c_str();
    response_message.BodyLength(std::strlen(r_message));
    std::memcpy(response_message.Body(), r_message, response_message.BodyLength());
    response_message.EncodeHeader();

    Write(response_message);
}

void Session::HandleInitRbf(Message& message)
{
    json::parser parser;
    parser.write(message.Body(), message.BodyLength());
    json::value value = parser.release();

    const RadialBasisFunctionCreateInfo createInfo = json::value_to<RadialBasisFunctionCreateInfo>(value);

    _radial_basis_function = new RadialBasisFunction(createInfo.InputNumbers, createInfo.RadialCenters, this);

    OnInitRbf();
}

void Session::OnInitRbf()
{

}

void Session::HandleStartTrainingRbf(Message& message)
{
    json::parser parser;
    parser.write(message.Body(), message.BodyLength());
    json::value value = parser.release();

    const RadialBasisFunctionTrainingInfo trainingInfo = json::value_to<RadialBasisFunctionTrainingInfo>(value);

    Message trainMessage;
    trainMessage.Opcode(SMSG_TRAIN_SELF_ORG_MAP);
    const char* r_message;
    if (_radial_basis_function)
    {
        if (-_radial_basis_function->Train(trainingInfo))
        {
            std::cout << "[server]: Training success!" << std::endl;
            r_message = "true";
        } else
        {
            std::cout << "[server]: Not training!" << std::endl;
            r_message = "false";
        }

        trainMessage.BodyLength(std::strlen(r_message));
        std::memcpy(trainMessage.Body(), r_message, trainMessage.BodyLength());
        trainMessage.EncodeHeader();

        Write(trainMessage);
    } else
    {
        std::cout << "[server]: Radial basis function network not init" << std::endl;
    }
}

void Session::HandleSimulateDataRbf(Message& message)
{
    json::parser parser;
    parser.write(message.Body(), message.BodyLength());
    json::value value = parser.release();

    const std::vector<double> inputs = json::value_to<std::vector<double>>(value);
    double output;

    if (_radial_basis_function)
    {
        _radial_basis_function->GetOutput(inputs, &output);

        Message responseMessage;
        responseMessage.Opcode(SMSG_SIMULATE_DATA_RBF);

        json::value responseValue = json::value_from(output);
        std::string response_str = json::serialize(responseValue);

        const char* r_message = response_str.c_str();

        responseMessage.BodyLength(std::strlen(r_message));
        std::memcpy(responseMessage.Body(), r_message, responseMessage.BodyLength());

        responseMessage.EncodeHeader();

        Write(responseMessage);
    } else
    {
        std::cout << "[server]: Radial basis function network not init!";
    }
}
