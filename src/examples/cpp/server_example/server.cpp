
#include <iostream>
#include <future>
#include <thread>

#include <MapServer.hpp>
#include <MapApi.hpp>

#define SERVER_PORT 5140
using namespace MAP;

int main(int argc, char *argv[])
{
    try
    {
        MapServer(EncodingMethod::MAP_BINARY, TransportMethod::ASIO_SOCKETS, true, SERVER_PORT);
    }
    catch (std::exception &e)
    {
        std::cerr << "Cannot initialize map server example due:" << e.what() << "\n";
    }
    return 0;
}