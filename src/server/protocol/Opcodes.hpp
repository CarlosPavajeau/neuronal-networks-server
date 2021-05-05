//
// Created by cantte on 26/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_OPCODES_HPP
#define NEURONAL_NETWORKS_SERVER_OPCODES_HPP

#include "Define.hpp"

#include <iomanip>
#include <vector>
#include <sstream>

#define GET_OPCODE_NAME(opcode) #opcode

enum Opcodes : uint16
{
    CMSG_INIT_MADELINE = 0x001,
    SMSG_INIT_MADELINE = 0x002,
    CMSG_TRAIN_MADELINE = 0x003,
    SMSG_TRAIN_MADELINE = 0x004,
    CMSG_SIMULATE_DATA_MADELINE = 0x005,
    SMSG_SIMULATE_DATA_MADELINE = 0x006,
    CMSG_INIT_SELF_ORG_MAP = 0x007,
    SMSG_INIT_SELF_ORG_MAP = 0x008,
    CMSG_TRAIN_SELF_ORG_MAP = 0x009,
    SMSG_TRAIN_SELF_ORG_MAP = 0x010,
    CMSG_SIMULATE_DATA_SELF_ORG_MAP = 0x011,
    SMSG_SIMULATE_DATA_SELF_ORG_MAP = 0x012,
    NUM_MSG_TYPES = 0x013
};

enum OpcodeMisc : uint16
{
    NUM_OPCODE_HANDLERS = NUM_MSG_TYPES,
    NULL_OPCODE = 0x0000
};

typedef Opcodes OpcodeClient;
typedef Opcodes OpcodeServer;

class Session;

class Message;

class OpcodeHandler
{
public:
    explicit OpcodeHandler(char const* name) : Name(name) {}

    char const* Name;
};

class ClientOpcodeHandler : public OpcodeHandler
{
public:
    explicit ClientOpcodeHandler(char const* name) : OpcodeHandler(name) {}

    virtual void Call(Session* session, Message& message) const = 0;
};

#endif //NEURONAL_NETWORKS_SERVER_OPCODES_HPP
