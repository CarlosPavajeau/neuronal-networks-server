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
};

std::vector<std::string> opCodesNames
        {
                GET_OPCODE_NAME(MSGC_INIT_NEURON),
                GET_OPCODE_NAME(MSGS_INIT_NEURON),
        };

template<typename T>
inline std::string GetOpcodeNameForLoggingImpl(T id)
{
    uint16 opcode = uint16(id);
    std::ostringstream ss;
    ss << '[';

    ss << opCodesNames[opcode - 1];

    ss << " 0x" << std::hex << std::setw(4) << std::setfill('0') << std::uppercase << opcode << std::nouppercase
       << std::dec << " (" << opcode << ")]";

    return ss.str();
}

std::string GetOpCodeName(Opcodes opcode)
{
    return GetOpcodeNameForLoggingImpl(opcode);
}

#endif //NEURONAL_NETWORKS_SERVER_OPCODES_HPP
