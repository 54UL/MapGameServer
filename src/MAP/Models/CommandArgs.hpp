#ifndef COMMANDARGS_H
#define COMMANDARGS_H
#include <string>
#include <string>
#include "Client.hpp"



namespace MAP
{
    class CommandArgs
    {
    public:
        CommandArgs(std::shared_ptr<MAP::Client> owner, const uint8_t * args) : Owner(owner), data(args)
        {
        }
        std::shared_ptr<MAP::Client> Owner;
        uint8_t Args;
    };

} // namespace MAP

#endif