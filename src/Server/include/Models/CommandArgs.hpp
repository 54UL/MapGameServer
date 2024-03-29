#ifndef COMMANDARGS_H
#define COMMANDARGS_H

#include <string>
#include <memory>
#include "./Client.hpp"
#include <BinaryUtils.hpp>
#include <SerializerAPI.hpp>

namespace MAP
{
    class CommandArgs
    {
    public:
        CommandArgs(std::shared_ptr<MAP::Client> owner, std::map<std::string, std::shared_ptr<MAP::IMapObject>>  payload) : Owner(owner), Payload(payload)
        {

        }

        std::shared_ptr<MAP::Client> Owner;
        std::map<std::string, std::shared_ptr<MAP::IMapObject>> Payload;
    };

} // namespace MAP

#endif