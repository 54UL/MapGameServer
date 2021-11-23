#ifndef UPSERT_COMMAND_H
#define UPSERT_COMMAND_H
#include <spdlog/spdlog.h>
#include "../../include/MapServer.hpp"
#include "../../include/MapApi.hpp"

namespace MAP
{
    class Upsert : public IMapCommand
    {
    public:
        Upsert() {}
        ~Upsert() {}

        void Evalute(MapServer *context, CommandArgs &args)
        {
            auto key = args.Payload["Key"]->GetInt32();
            auto value = args.Payload["Value"];
            context->UpsertValue(0, key, value);
            //build response
            auto dataFormater = context->GetCurrentDataFormat();
            std::vector<std::shared_ptr<MAP::IMapObject>>
                commandPayload = {
                    dataFormater->CreateObject(key, "Key")};

            context->UpsertValue(0, key, value);
            context->PushCommand(static_cast<uint8_t>(ServerCommandType::UPSERT), true, commandPayload, args.Owner);
        }

        uint32_t Id() override
        {
            return static_cast<uint32_t>(ServerCommandType::UPSERT);
        }
    };
}

#endif