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
    MSGC_INIT_NEURON = 0x001,
    MSGS_INIT_NEURON = 0x002,
    MSGC_TRAIN_NEURON = 0x003,
    MSGS_TRAIN_NEURON = 0x004,
    NUM_MSG_TYPES = 0x005
};

enum OpcodeMisc : uint16
{
    NUM_OPCODE_HANDLERS = NUM_MSG_TYPES,
    NULL_OPCODE = 0x0000
};

typedef Opcodes OpcodeClient;

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
