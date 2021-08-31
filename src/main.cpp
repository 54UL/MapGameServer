
#include <iostream>
#include <future>
#include <thread>
#include "./MAP/MapServer.hpp"
#include "./MAP/UnitTesting.hpp"
#include "./MAP/Tests/SerializerTest.hpp"

#define SERVER_PORT 5140

int main(int argc, char *argv[])
{
    try
    {
#ifdef RUN_TESTS
        MAP::MapTester::AddTest<MAP::SerializerTest>();
        if (MAP::MapTester::RunAll())
        {
            std::cout << "ALL TESTS SUCCEDED...\n";
        }
        else{
            std::cout << "SOME TEST FAILED\n";
        }
        std::cin.get();
#endif

        asio::io_context io_context;
        MAP::MapServer(io_context, SERVER_PORT);
    }
    catch (std::exception &e)
    {
        std::cerr << "Cannot initialize MAP server due:" << e.what() << "\n";
    }
    return 0;
}