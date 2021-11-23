#ifndef SERVER_COMMAND_H
#define SERVER_COMMAND_H
#include <string>
#include <memory>
#include <vector>

#include "./Client.hpp"
#include "../IMapObject.hpp"

namespace MAP
{
    class Command
    {
    public:
        Command(const uint8_t code, std::vector<std::shared_ptr<MAP::IMapObject>> payLoad, bool broadcast, std::shared_ptr<MAP::Client> owner)
            : Code(code), PayLoad(payLoad), IsBroadCast(broadcast), Owner(owner)
        {
        }
        ~Command()
        {
        }
        uint32_t Code;
        bool IsBroadCast;
        std::vector<std::shared_ptr<MAP::IMapObject>> PayLoad;
        std::shared_ptr<MAP::Client> Owner;
    };

} // namespace MAP

#endif