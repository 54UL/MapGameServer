#ifndef COMMANDARGS_H
#define COMMANDARGS_H

#include <string>
#include <memory>
#include "./Client.hpp"
#include "../Serialization/BinaryObject.hpp"
#include "../Serialization/SerializerAPI.hpp"

namespace MAP
{
    class CommandArgs
    {
    public:
        CommandArgs(std::shared_ptr<MAP::Client> owner, std::vector<std::shared_ptr<MAP::INetworkType>> args) : Owner(owner), Args(args)
        {

        }
        std::shared_ptr<MAP::Client> Owner;
        std::vector<std::shared_ptr<MAP::INetworkType>> Args;
    };

} // namespace MAP

#endif