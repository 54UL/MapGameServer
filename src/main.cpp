
#include <iostream>
#include <future>
#include <thread>

#include "./MAP/MapServer.hpp"

int main(int argc, char *argv[])
{
    try
    {
        asio::io_context io_context;
        MapServer server(io_context, 5140);
    }
    catch (std::exception &e)
    {
        std::cerr << "cannot initialize MAP server:" << e.what() << "\n";
    }
    return 0;
}