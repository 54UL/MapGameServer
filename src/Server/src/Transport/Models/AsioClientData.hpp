#ifndef ASIO_CLIENT_DATA_H
#define ASIO_CLIENT_DATA_H

#include <asio.hpp>

using asio::ip::udp;

namespace MAP
{
    class AsioClientData
    {
    public:
        AsioClientData(udp::endpoint endpoint, int port){
            
        }
        ~AsioClientData()
        {
        }

       
        int Port;
    };
}

#endif