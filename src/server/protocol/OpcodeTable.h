//
// Created by cantte on 27/04/21.
//

#ifndef NEURONAL_NETWORKS_SERVER_OPCODETABLE_H
#define NEURONAL_NETWORKS_SERVER_OPCODETABLE_H

#include "Opcodes.hpp"

class OpcodeTable
{
public:
    OpcodeTable();

    OpcodeTable(OpcodeTable const&) = delete;

    OpcodeTable& operator=(OpcodeTable const&) = delete;

    ~OpcodeTable();

    void Initialize();

    ClientOpcodeHandler const* operator[](Opcodes index) const
    {
        return _internalTableClient[index];
    }

    ClientOpcodeHandler const* GetClientOpcodeHandler(Opcodes index) const
    {
        return _internalTableClient[index];
    }

private:
    template<typename Handler, Handler HandlerFunction>
    void ValidateAndSetClientOpcode(OpcodeClient opcode, char const* name);

    ClientOpcodeHandler* _internalTableClient[NUM_OPCODE_HANDLERS];
};

extern OpcodeTable opcodeTable;

/// Lookup opcode name for human understandable logging
std::string GetOpcodeNameForLogging(Opcodes opcode);

#endif //NEURONAL_NETWORKS_SERVER_OPCODETABLE_H
