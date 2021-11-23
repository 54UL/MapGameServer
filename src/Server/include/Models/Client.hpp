#ifndef CLIENT_H
#define CLIENT_H
#include "../MapApi.hpp"
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
        Client(int32_t userid, std::string &playerName) : UserId(userid), PlayerName(playerName)
        {
        }
        ~Client() {}
        int32_t UserId;
        std::string PlayerName;
        std::string HostName;
        MAP::Vector<MAP::SpawnedEntity> CurrentSpawnedObjects;
    };
} // namespace MAP

#endif