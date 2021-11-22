#ifndef IMAP_COMMAND_H
#define IMAP_COMMAND_H

#include "./MapServer.hpp"
#include "./Models/CommandArgs.hpp"

namespace MAP
{
    class IMapCommand
    {
    public:
        IMapCommand();
        virtual ~IMapCommand();

        virtual void Evalute(MapServer *context, const CommandArgs &args) = 0;
        virtual int32_t Id() = 0;
    };
}

#endif