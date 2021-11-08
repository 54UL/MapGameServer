#ifndef CLIENT_H
#define CLIENT_H
#include "../Api.hpp"
#include "./SpawnedEntity.hpp"

#include <string>
#include <asio.hpp>

using asio::ip::udp;
namespace MAP
{
    class Client
    {
    public:
        Client() : UserId(0) {}
        Client(uint64_t userid, std::string &playerName, std::string &hostName, udp::endpoint endpoint, uint32_t port) : UserId(userid), PlayerName(playerName), HostName(hostName), ClientEndpoint(endpoint), Port(port)
        {
        }
        ~Client() {}
        uint64_t UserId;
        std::string PlayerName;
        std::string HostName;
        uint32_t Port;
        udp::endpoint ClientEndpoint;
        MAP::Vector<MAP::SpawnedEntity> CurrentSpawnedObjects;
    };
} // namespace MAP

#endif