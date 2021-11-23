#ifndef SPAWN_COMMAND_H
#define SPAWN_COMMAND_H
#include <spdlog/spdlog.h>
#include "../../include/MapServer.hpp"
#include "../../include/MapApi.hpp"

namespace MAP
{
    class Spawn : public IMapCommand
    {
    public:
        Spawn() {}
        ~Spawn() {}

        void Evalute(MapServer *context, CommandArgs &args)
        {
            auto playerId = args.Payload["PlayerId"]->GetInt32();
            auto prefabName = args.Payload["PrefabName"]->GetString();

            MAP::SpawnedEntity spawnedEntity(prefabName, playerId);

            auto dataFormater = context->GetCurrentDataFormat();

            std::vector<std::shared_ptr<MAP::IMapObject>> commandPayload = {
                dataFormater->CreateObject(playerId, "PlayerId"),
                dataFormater->CreateObject(prefabName, "PrefabName")};

            context->SpawnObject(555, spawnedEntity);
            context->PushCommand(static_cast<uint8_t>(ServerCommandType::SPAWN), true, commandPayload, args.Owner);
        }

        uint32_t Id() override
        {
            return static_cast<uint32_t>(ServerCommandType::SPAWN);
        }
    };
}

#endif