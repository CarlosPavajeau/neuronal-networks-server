//
// Created by cantte on 27/04/21.
//

#include "OpcodeTable.h"
#include "Session.hpp"
#include "Message.hpp"

template<class MessageClass, void(Session::*HandlerFunction)(MessageClass&)>
class PacketHandler : public ClientOpcodeHandler
{
public:
    PacketHandler(char const* name) : ClientOpcodeHandler(name) {}

    void Call(Session* session, Message& message) const override
    {
        MessageClass niceMessage(std::move(message));
        (session->*HandlerFunction)(niceMessage);
    }
};

template<void(Session::*HandlerFunction)(Message&)>
class PacketHandler<Message, HandlerFunction> : public ClientOpcodeHandler
{
public:
    PacketHandler(char const* name) : ClientOpcodeHandler(name) {}

    void Call(Session* session, Message& message) const override
    {
        (session->*HandlerFunction)(message);
    }
};

OpcodeTable opcodeTable;

template<typename T>
struct get_message_class
{
};

template<typename MessageClass>
struct get_message_class<void (Session::*)(MessageClass&)>
{
    using type = MessageClass;
};

OpcodeTable::OpcodeTable()
{
    memset(_internalTableClient, 0, sizeof(_internalTableClient));
}

OpcodeTable::~OpcodeTable()
{
    for (uint16 i = 0; i < NUM_OPCODE_HANDLERS; ++i)
    {
        delete _internalTableClient[i];
    }
}

template<typename Handler, Handler HandlerFunction>
void OpcodeTable::ValidateAndSetClientOpcode(OpcodeClient opcode, const char* name)
{
    if (uint32(opcode) == NULL_OPCODE)
    {
        return;
    }

    if (uint32(opcode) >= NUM_OPCODE_HANDLERS)
    {
        return;
    }

    if (_internalTableClient[opcode] != nullptr)
    {
        return;
    }

    _internalTableClient[opcode] = new PacketHandler<typename get_message_class<Handler>::type, HandlerFunction>(name);
}

void OpcodeTable::ValidateAndSetServerOpcode(OpcodeServer opcode, const char* name)
{
    if (uint32(opcode) == NULL_OPCODE)
    {
        return;
    }

    if (uint32(opcode) >= NUM_OPCODE_HANDLERS)
    {
        return;
    }

    if (_internalTableClient[opcode] != nullptr)
    {
        return;
    }

    _internalTableClient[opcode] = new PacketHandler<Message, &Session::HandleServerSide>(name);
}

void OpcodeTable::Initialize()
{
#define DEFINE_HANDLER(opcode, handler) \
    ValidateAndSetClientOpcode<decltype(handler), handler>(opcode, #opcode)

#define DEFINE_SERVER_OP_CODE_HANDLER(opcode) \
    ValidateAndSetServerOpcode(opcode, #opcode)

    DEFINE_HANDLER(CMSG_INIT_MADELINE, &Session::HandleInitMadeline);
    DEFINE_HANDLER(CMSG_TRAIN_MADELINE, &Session::HandleStartTrainingMadeline);
    DEFINE_HANDLER(CMSG_SIMULATE_DATA_MADELINE, &Session::HandleSimulateDataMadeline);

    DEFINE_HANDLER(CMSG_INIT_SELF_ORG_MAP, &Session::HandleInitSelfOrgMap);
    DEFINE_HANDLER(CMSG_TRAIN_SELF_ORG_MAP, &Session::HandleStartTrainingSelfOrgMap);
    DEFINE_HANDLER(CMSG_SIMULATE_DATA_SELF_ORG_MAP, &Session::HandleSimulateDataSelfOrgMap);

    DEFINE_HANDLER(CMSG_INIT_RBF, &Session::HandleInitRbf);
    DEFINE_HANDLER(CMSG_TRAIN_RBF, &Session::HandleStartTrainingRbf);
    DEFINE_HANDLER(CMSG_SIMULATE_DATA_RBF, &Session::HandleSimulateDataRbf);

    DEFINE_SERVER_OP_CODE_HANDLER(SMSG_INIT_MADELINE);
    DEFINE_SERVER_OP_CODE_HANDLER(SMSG_TRAIN_MADELINE);
    DEFINE_SERVER_OP_CODE_HANDLER(SMSG_SIMULATE_DATA_MADELINE);

    DEFINE_SERVER_OP_CODE_HANDLER(SMSG_INIT_SELF_ORG_MAP);
    DEFINE_SERVER_OP_CODE_HANDLER(SMSG_TRAIN_SELF_ORG_MAP);
    DEFINE_SERVER_OP_CODE_HANDLER(SMSG_SIMULATE_DATA_SELF_ORG_MAP);

    DEFINE_SERVER_OP_CODE_HANDLER(SMSG_INIT_RBF);
    DEFINE_SERVER_OP_CODE_HANDLER(SMSG_TRAIN_RBF);
    DEFINE_SERVER_OP_CODE_HANDLER(SMSG_SIMULATE_DATA_RBF);

#undef DEFINE_HANDLER
#undef DEFINE_SERVER_OP_CODE_HANDLER
}

std::string GetOpcodeNameForLogging(Opcodes id)
{
    auto opcode = uint16(id);
    std::ostringstream ss;
    ss << '[';

    if (static_cast<uint16>(id) < NUM_OPCODE_HANDLERS)
    {
        if (OpcodeHandler const* handler = opcodeTable[id])
            ss << handler->Name;
        else
            ss << "UNKNOWN OPCODE";
    } else
        ss << "INVALID OPCODE";

    ss << " 0x" << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << opcode << std::nouppercase
       << std::dec << " (" << opcode << ")]";
    return ss.str();
}
