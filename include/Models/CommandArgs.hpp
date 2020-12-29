#ifndef COMMANDARGS_H
#define COMMANDARGS_H
#include <string>
#include <string>
#include "Client.hpp"

#include "../json.hpp"
using json = nlohmann::json;

namespace MAP
{
    class CommandArgs
    {
    public:
        CommandArgs(std::shared_ptr<MAP::Client> owner, json args) : Owner(owner), data(args)
        {
        }
        std::shared_ptr<MAP::Client> Owner;
        json data;
    };

} // namespace MAP

#endif