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
        CommandArgs(std::shared_ptr<MAP::Client> owner, std::map<std::string, std::shared_ptr<MAP::INetworkType>>  payload) : Owner(owner), Payload(payload)
        {

        }
        std::shared_ptr<MAP::Client> Owner;
        std::map<std::string, std::shared_ptr<MAP::INetworkType>> Payload;
    };

} // namespace MAP

#endif