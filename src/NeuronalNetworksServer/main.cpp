#include "Server.hpp"
#include "Config.h"
#include "OpcodeTable.h"

#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/log/trivial.hpp>

namespace fs = boost::filesystem;

#ifndef _NEURONAL_NETWORK_CONFIG
#define _NEURONAL_NETWORK_CONFIG "neuronalserver.conf"
#endif

int main(int argc, char* argv[])
{

    try
    {
        auto confFile = fs::absolute(_NEURONAL_NETWORK_CONFIG);
        std::string configError;
        if (!sConfigMgr->LoadInitial(confFile.generic_string(), std::vector<std::string>(argv, argv + argc),
                                     configError))
        {
            printf("Error in config file: %s\n", configError.c_str());
            return 1;
        }
        BOOST_LOG_TRIVIAL(info) << "Using configuration file " << sConfigMgr->GetFilename().c_str();

        boost::asio::io_context io_context;
        tcp::endpoint endpoint(tcp::v4(), sConfigMgr->GetIntDefault("Port", 3000));

        BOOST_LOG_TRIVIAL(info) << "Initializing the opcode table";
        opcodeTable.Initialize();
        BOOST_LOG_TRIVIAL(info) << "Initialized opcode table";

        Server server(io_context, endpoint);

        io_context.run();

    } catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
