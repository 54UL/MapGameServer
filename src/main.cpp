
#include <iostream>
#include <future>
#include <thread>
#include "../include/Api.hpp"
#include "../include/MapServer.hpp"
#include "../include/json.hpp"
#include "../include/Models/Client.hpp"

int main(int argc, char *argv[])
{
    try
    {
        asio::io_context io_context;
        MapServer server(io_context, 5140);
    }
    catch (std::exception &e)
    {
        std::cerr << "Exception ='(  :" << e.what() << "\n";
    }
    return 0;
}