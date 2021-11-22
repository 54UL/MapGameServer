#ifndef SUBSCRIBE_COMMAND_H
#define SUBSCRIBE_COMMAND_H
#include <spdlog/spdlog.h>
#include "../../include/IMapCommand.hpp"
namespace MAP
{
    class Subscribe : public IMapCommand
    {
    public:
        Subscribe();
        virtual ~Subscribe();

        void Evalute(MapServer *context, const CommandArgs &args)
        {

            // std::string IpAddress = BinaryUtils::Get<MAP::NetString>(args.Payload, "IpAddress")->GetValue();
            // std::string playerName = BinaryUtils::Get<MAP::NetString>(args.Payload, "PlayerName")->GetValue();
            // std::string hostName = BinaryUtils::Get<MAP::NetString>(args.Payload, "HostName")->GetValue();

            std::string IpAddress = args.Payload["IpAddress"]->GetString();
            std::string playerName = args.Payload["PlayerName"]->GetString();
            std::string hostName = args.Payload["HostName"]->GetString();
            int port = args.Payload["Port"]->GetInt32();

            if (IpAddress.compare("") == 0)
                return;

            if (connectedClients_.size() < MAX_CLIENTS)
            {
                auto userId = lastClientIndex++;
                // auto ipAddrr = asio::ip::address::from_string(IpAddress);
                // auto clientEndpoint = udp::endpoint(ipAddrr, port);

                auto newClient = std::make_shared<MAP::Client>(userId, playerName);
                connectedClients_.emplace_back(newClient);

                //SEND REPLY TODO: ADD NEW SERIALIZATION API
                NetworkObject commandPayload;
                NetworkObject spawnedObjs;

                for (const auto &entity : spawnedObjects_)
                {
                    NetworkObject entityStructure{
                        std::make_shared<MAP::NetString>(entity.PrefabName, "PrefabName"),
                        std::make_shared<MAP::NetInt>(entity.PlayerOwner, "PlayerId"),
                    };

                    spawnedObjs.insert(spawnedObjs.end(), entityStructure.begin(), entityStructure.end());
                }

                //TODO; DESING THE CREATING IMapObjects...
                commandPayload.push_back(std::make_shared<MAP::NetInt>(userId, "ClientId"));
                commandPayload.push_back(std::make_shared<MAP::NetString>("null", "AccesToken"));
                commandPayload.push_back(std::make_shared<MAP::NetArray>(spawnedObjs, "SpawnedEntities"));

                std::vector<MAP::IMapObject> payload;
                context->PushCommand(static_cast<uint8_t>(ServerCommandType::SUBSCRIBE), false, payload, connectedClients_.back());
                spdlog::info("user connected");
            }
        }
        
        int32_t Id() override
        {
        }
    };
}

#endif