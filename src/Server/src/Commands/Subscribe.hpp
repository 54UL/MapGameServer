#ifndef SUBSCRIBE_COMMAND_H
#define SUBSCRIBE_COMMAND_H
#include <spdlog/spdlog.h>
#include "../../include/MapServer.hpp"
#include "../../include/MapApi.hpp"

namespace MAP
{
    class Subscribe : public IMapCommand
    {
    public:
        Subscribe()
        {
        }
        ~Subscribe()
        {
        }

        void Evalute(MapServer *context, CommandArgs &args)
        {
            // auto IpAddress = args.Payload["IpAddress"]->GetString();
            auto IpAddress = args.Payload["IpAddress"]->GetString();
            auto playerName = args.Payload["PlayerName"]->GetString();
            auto port = args.Payload["Port"]->GetString();

            if (IpAddress.compare("") == 0)
                return;

            if (context->ConnectedClients() < MAX_CLIENTS)
            {
                auto userId = context->ConnectedClients();

                auto newClient = std::make_shared<MAP::Client>(userId, playerName);
                auto addedClient = context->AddClient(newClient);

                std::vector<std::shared_ptr<MAP::IMapObject>> spawnedObjs;
                auto dataFormater = context->GetCurrentDataFormat();

                for (const auto &entity : context->GetPoolSpawnedEntities(0)) //This only if the requested joining session is created or has items...
                {
                    std::vector<std::shared_ptr<MAP::IMapObject>> entityStructure{
                        dataFormater->CreateObject(entity.PrefabName, "PrefabName"),
                        dataFormater->CreateObject(entity.PlayerOwner, "PlayerId"),
                    };

                    spawnedObjs.insert(spawnedObjs.end(), entityStructure.begin(), entityStructure.end());
                }

                std::vector<std::shared_ptr<MAP::IMapObject>> payLoad = {
                    dataFormater->CreateObject(userId, "ClientId"),
                    dataFormater->CreateObject("userId", "AccesToken"),
                    dataFormater->CreateObject(spawnedObjs, "SpawnedEntities")};
                spdlog::info("USER CONNECTED... ID:{}", userId);
                context->PushCommand(static_cast<uint8_t>(ServerCommandType::SUBSCRIBE), false, payLoad, addedClient);
            }
        }

        uint32_t Id() override
        {
            return static_cast<uint32_t>(ServerCommandType::SUBSCRIBE);
        }
    };
}

#endif