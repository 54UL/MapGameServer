#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H
#include <string>
#include <string>
#include "Client.hpp"
#include <memory>
#include <vector>

#include <BinaryUtils.hpp>
#include "SerializerAPI.hpp"

namespace MAP
{
    class Command
    {
    public:
        Command(const uint8_t code, NetworkObject payLoad,bool broadcast, std::shared_ptr<MAP::Client> owner) 
        : Code(code), PayLoad(payLoad), IsBroadCast(broadcast), Owner(owner)
        {
        }
        ~Command()
        {
        }
        uint8_t Code;
        bool IsBroadCast;
        NetworkObject PayLoad;
        std::shared_ptr<MAP::Client> Owner;
    };

} // namespace MAP

#endif