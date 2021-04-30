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
    CMSG_TRAIN_NEURON = 0x003,
    SMSG_TRAIN_NEURON = 0x004,
    CMSG_SIMULATE_DATA = 0x005,
    SMSG_SIMULATE_DATA = 0x006,
    NUM_MSG_TYPES = 0x007
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
    OpcodeHandler(char const* name) : Name(name) {}

    char const* Name;
};

class ClientOpcodeHandler : public OpcodeHandler
{
public:
    ClientOpcodeHandler(char const* name) : OpcodeHandler(name) {}

    virtual void Call(Session* session, Message& message) const = 0;
};

#endif //NEURONAL_NETWORKS_SERVER_OPCODES_HPP
