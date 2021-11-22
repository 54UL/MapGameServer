#include <mutex>
#include <thread>
#include <asio.hpp>
#include <spdlog/spdlog.h>
#include <BinaryUtils.hpp>
#include <SerializerAPI.hpp>
#include <Types/Command.hpp>
#include "../include/MapServer.hpp"

namespace MAP
{
    MapServer::MapServer(EncodingMethod encoding, TransportMethod transport, bool useDefaultCommands, short port) : m_current_format(encoding),
                                                                                                                    m_current_transport(transport, port),
                                                                                                                    m_commands_manager(useDefaultCommands),
                                                                                                                    lastClientIndex(32)
    {

        //         auto serverStartTime = std::chrono::high_resolution_clock::now();
        //         // serializer_ = std::make_shared<MAP::BinaryUtils>();
        // #ifndef SINGLE_THREAD
        //         MultiThread(io_context);
        // #else
        //         SingleThread(io_context);
        // #endif
        //         Initialize();
        //         std::cout << "SERVER RUNNING..." << std::endl;
    }

    MapServer::~MapServer()
    {
        receiverThread_.join();
        dispatcherThread_.join();
    }

    void MapServer::MultiThread()
    {
        spdlog::info("DUAL THREAD MODE");
        receiverThread_ = std::thread([&]()
                                      {
                                          while (1)
                                          {
                                              //Recive...
                                          }
                                      });
        dispatcherThread_ = std::thread([&]()
                                        {
                                            while (1)
                                            {
                                                //Tick server
                                            }
                                        });
    }

    void MapServer::SingleThread()
    {
        spdlog::info("SINGLE THREAD MODE");
        while (1)
        {
            // ReceiveData();
            // TickServer();
        }
    }

    void MapServer::Initialize()
    {
    }

    void MapServer::PushCommand(uint32_t commandId, bool broadcast, std::vector<IMapObject> payLoad, std::shared_ptr<MAP::Client> owner)
    {
        commandMutex_.lock();
        commandQueue_.push_back(MAP::Command(commandId, payLoad, broadcast, owner));
        commandMutex_.unlock();
    }

    // void MapServer::RegisterCommands()
    // {
    //     RegisterCommand(MAP::ServerCommandType::SUBSCRIBE, std::bind(&MapServer::Subscribe, this, std::placeholders::_1));
    //     RegisterCommand(MAP::ServerCommandType::UNSUBSCRIBE, std::bind(&MapServer::Unsubscribe, this, std::placeholders::_1));
    //     RegisterCommand(MAP::ServerCommandType::START_POOL, std::bind(&MapServer::StartPool, this, std::placeholders::_1));
    //     RegisterCommand(MAP::ServerCommandType::END_POOL, std::bind(&MapServer::EndPool, this, std::placeholders::_1));
    //     RegisterCommand(MAP::ServerCommandType::UPSERT, std::bind(&MapServer::UpsertProperty, this, std::placeholders::_1));
    //     RegisterCommand(MAP::ServerCommandType::REMOVE, std::bind(&MapServer::RemoveProperty, this, std::placeholders::_1));
    //     RegisterCommand(MAP::ServerCommandType::SPAWN, std::bind(&MapServer::SpawnObject, this, std::placeholders::_1));
    //     RegisterCommand(MAP::ServerCommandType::GET_ACTIVE_POOLS, std::bind(&MapServer::GetActivePools, this, std::placeholders::_1));
    // }

    // void MapServer::RegisterCommand(MAP::ServerCommandType code, std::function<void(MAP::CommandArgs &payload)> callback)
    // {
    //     commands_.insert(std::make_pair(code, callback));
    // }

    // void MapServer::TickServer()
    // {
    //     // auto tickStartTime = std::chrono::high_resolution_clock::now();
    //     DispatchClientComands();
    //     std::this_thread::sleep_for(std::chrono::milliseconds(1));
    // }

    bool MapServer::ShouldSendData(const MAP::Command &command, std::shared_ptr<MAP::Client> client)
    {
        if (command.IsBroadCast)
            return command.Owner->UserId != client->UserId;
        else
            return command.Owner->UserId == client->UserId;
    }

    void MapServer::DispatchClientComands()
    {
        auto queueCopy = commandQueue_;
        for (const auto &commands : commandQueue_)
        {
            if (!queueCopy.empty())
            {
                auto top = queueCopy.back();
                for (const auto &client : connectedClients_)
                {
                    if (ShouldSendData(top, client))
                    {
                        SendToClient(top, client);
                    }
                }
                queueCopy.pop_back();
            }
        }

        commandMutex_.lock();
        if (!commandQueue_.empty())
        {
            commandQueue_.pop_back();
        }
        commandMutex_.unlock();
    }

    //TODO; CASO PARA CUANDO NO EXISTA USUARIO Y IMPLEMENTAR CACHE
    std::shared_ptr<MAP::Client> MapServer::GetCommandInfo(int clientId)
    {
        if (clientId <= 0)
            return nullptr;

        auto client = std::find_if(connectedClients_.begin(), connectedClients_.end(), [&](std::shared_ptr<MAP::Client> e)
                                   {
                                       if (e->UserId == clientId)
                                           return true;
                                       else
                                           return false;
                                   });
        return *client;
    }

    void MapServer::OnRecive(const uint8_t *data, std::size_t length)
    {
        /*
            DRAFT FOR NEW MAP API
            std::vector<std::shared_ptr<IMapObject>> dataSequence = formatManager->Decode(data_,length);
            std::shared_ptr<IMapObject> command = dataSequence.at(0);
            auto clientId = command.getInt32("clientId");
            auto commandId = command.getByte("id");
            dataSequence.erase(dataSequence.begin()); //OJO
            MAP::CommandArgs commandArg(GetCommandInfo(clientId), dataSequence);
            DecodeCommand(static_cast<MAP::ServerCommandType>(commandId), commandArg);
        */

        // auto dataSequence = BinaryUtils::DecodeAsMap(data_, length);
        // auto decodedCommand = std::dynamic_pointer_cast<MAP::NetCommand>((dataSequence.at(0)));
        // dataSequence.erase(dataSequence.begin());
        // MAP::CommandArgs commandArg(GetCommandInfo(decodedCommand->clientId()), dataSequence);
        // DecodeCommand(static_cast<MAP::ServerCommandType>(decodedCommand->id()), commandArg);
    }

    void MapServer::SendToClient(const MAP::Command &command, std::shared_ptr<MAP::Client> client)
    {
        NetworkObject objStructure;
        objStructure.push_back(std::make_shared<MAP::NetCommand>(command.Code, client->UserId));
        objStructure.insert(objStructure.begin(), command.PayLoad.begin(), command.PayLoad.end());
        std::vector<uint8_t> memoryBuffer = BinaryUtils::Encode(objStructure);
        // SendData(memoryBuffer.data(), memoryBuffer.size(), client->ClientEndpoint); TODO REMPLAZAR
    }


    // void MapServer::GetActivePools(MAP::CommandArgs &args)
    // {
    //     //test elements
    //     NetworkObject poolData{
    //         std::make_shared<MAP::NetInt>(666, "PoolId"),
    //         std::make_shared<MAP::NetString>("DEFAULT POOL", "PoolName")};

    //     NetworkObject
    //         commandPayload{
    //             std::make_shared<MAP::NetArray>(poolData, "PoolObj1"),
    //             std::make_shared<MAP::NetArray>(poolData, "PoolObj2"),
    //             std::make_shared<MAP::NetArray>(poolData, "PoolObj3"),
    //             std::make_shared<MAP::NetArray>(poolData, "PoolObj4")};

    //     commandMutex_.lock();
    //     commandQueue_.push_back(MAP::Command(static_cast<uint8_t>(ServerCommandType::GET_ACTIVE_POOLS), commandPayload, false, args.Owner));
    //     commandMutex_.unlock();
    // }

    // void MapServer::Unsubscribe(MAP::CommandArgs &args)
    // {
    // }

    // void MapServer::StartPool(MAP::CommandArgs &args)
    // {
    // }

    // void MapServer::EndPool(MAP::CommandArgs &args)
    // {
    // }

    // void MapServer::UpsertProperty(MAP::CommandArgs &args)
    // {
    //     std::string payloadKey = BinaryUtils::Get<MAP::NetString>(args.Payload, "Key")->GetValue();
    //     auto payloadValue = args.Payload["Value"];
    //     //Set internal data
    //     testingPool_[payloadKey] = payloadValue;
    //     //Broadcast payload
    //     NetworkObject commandPayload{
    //         std::make_shared<MAP::NetString>(payloadKey, "Key"),
    //         payloadValue};

    //     commandMutex_.lock();
    //     commandQueue_.push_back(MAP::Command(static_cast<uint8_t>(ServerCommandType::UPSERT), commandPayload, true, args.Owner));
    //     commandMutex_.unlock();
    // }

    // void MapServer::RemoveProperty(MAP::CommandArgs &args)
    // {
    // }

    // void MapServer::SpawnObject(MAP::CommandArgs &args)
    // {
    //     int playerId = BinaryUtils::Get<MAP::NetInt>(args.Payload, "PlayerId")->GetValue();
    //     std::string prefabName = BinaryUtils::Get<MAP::NetString>(args.Payload, "PrefabName")->GetValue();
    //     MAP::SpawnedEntity spawnedEntity(prefabName, playerId);
    //     NetworkObject commandPayload{
    //         args.Payload["PlayerId"],
    //         args.Payload["PrefabName"]};
    //     commandMutex_.lock();
    //     spawnedObjects_.push_back(spawnedEntity);
    //     commandQueue_.push_back(MAP::Command(static_cast<uint8_t>(ServerCommandType::SPAWN), commandPayload, true, args.Owner));
    //     commandMutex_.unlock();
    // }
}