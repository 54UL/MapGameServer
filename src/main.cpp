
#include <iostream>
#include <future>
#include <thread>
#include "../include/Api.hpp"
#include "../include/MapServer.hpp"
#include "../include/json.hpp"
#include "../include/Models/Client.hpp"



class some{
    public :
    
    some(){std::cout<<"empty constructed"<<std::endl;}
    some(int value):valueA(value){}
    ~some(){}

    int valueA;
    int valueB;
    std::string lol;
};


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