#ifndef IMAP_COMMAND_H
#define IMAP_COMMAND_H

// #include "./MapServer.hpp"
#include "./Models/CommandArgs.hpp"

namespace MAP
{
    class MapServer;
    class IMapCommand
    {
    public:
        IMapCommand(){}
        virtual ~IMapCommand(){

        }

        virtual void Evalute(MapServer *context, CommandArgs &args) = 0;
        virtual uint32_t Id() = 0; //REMOVE THIS MF
    };
}

#endif