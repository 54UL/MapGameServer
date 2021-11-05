
#include <iostream>
#include <future>
#include <thread>
#include <MapServer.hpp>

#define SERVER_PORT 5140

int main(int argc, char *argv[])
{
    try
    {
        asio::io_context io_context;
        MAP::MapServer(io_context, SERVER_PORT);
    }
    catch (std::exception &e)
    {
        std::cerr << "Cannot initialize MAP server due:" << e.what() << "\n";
    }
    return 0;
}