#ifndef UNSUBSCRIBE_COMMAND_H
#define UNSUBSCRIBE_COMMAND_H
#include <spdlog/spdlog.h>
#include "../../include/MapServer.hpp"
#include "../../include/MapApi.hpp"

namespace MAP
{
    class Unsubscribe : public IMapCommand
    {
    public:
        Unsubscribe(){

        }
        ~Unsubscribe(){

        }

        void Evalute(MapServer *context, CommandArgs &args)
        {
          
        }

        uint32_t Id() override
        {
            return static_cast<uint32_t>(ServerCommandType::UNSUBSCRIBE);
        }
    };
}

#endif