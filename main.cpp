#include <iostream>

#include "Server.hpp"

int main(int argc, char* argv[])
{

    try
    {
        if (argc < 2 || argc > 2)
        {
            std::cerr << "Usage neuronal_networks_server <port>";
            return 1;
        }

        boost::asio::io_context io_context;
        tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[1]));

        Server server(io_context, endpoint);

        io_context.run();

    } catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
