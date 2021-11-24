
#include <iostream>
#include <future>
#include <thread>

#include <MapServer.hpp>
#include <MapApi.hpp>

using namespace MAP;

int main(int argc, char *argv[])
{
    bool VanillaCommands = true;
    int ServerPort = 5408;
    
    if (argc == 2)
    {
        //Add command line args.... xd
    }

    MapServer vanillaServer(EncodingMethod::MAP_BINARY, TransportMethod::ASIO_SOCKETS, ServerPort);
    vanillaServer.Initialize(VanillaCommands);
    return vanillaServer.Run();
}