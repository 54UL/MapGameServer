#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <string>
#include "Client.hpp"
#include <memory>


namespace MAP
{
    class Command
    {
    public:
        Command(const std::string &header, const char* payLoad, bool broadcast, std::shared_ptr<MAP::Client> owner) : Header(header), PayLoad(payLoad), IsBroadCast(broadcast), Owner(owner)
        {
        }
        ~Command()
        {
        }
        std::string Header;
        bool IsBroadCast;
       const char* PayLoad;
        std::shared_ptr<MAP::Client> Owner;
    };

} // namespace MAP

#endif