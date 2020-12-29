#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include "../json.hpp"
#include <string>
#include "Client.hpp"
#include <memory>

using json = nlohmann::json;

namespace MAP
{
    class Command
    {
    public:
        Command(const std::string &header, json payLoad, bool broadcast, std::shared_ptr<MAP::Client> owner) : Header(header), PayLoad(payLoad), IsBroadCast(broadcast), Owner(owner)
        {
        }
        ~Command()
        {
        }
        std::string Header;
        bool IsBroadCast;
        json PayLoad;
        std::shared_ptr<MAP::Client> Owner;
    };

} // namespace MAP

#endif