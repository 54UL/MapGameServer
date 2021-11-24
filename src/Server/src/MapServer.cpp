#include <mutex>
#include <thread>
#include <asio.hpp>
#include <spdlog/spdlog.h>
#include <BinaryUtils.hpp>
#include <SerializerAPI.hpp>
#include <Types/Command.hpp>
#include "../include/MapServer.hpp"

#include "./Commands/Subscribe.hpp"
#include "./Commands/Unsubscribe.hpp"
#include "./Commands/Upsert.hpp"
#include "./Commands/ActivePools.hpp"
#include "./Commands/Spawn.hpp"

namespace MAP
{
    MapServer::MapServer(EncodingMethod encoding, TransportMethod transport, short port) : m_current_format(encoding),
                                                                                           m_current_transport(transport, port),
                                                                                           m_commands_manager(),
                                                                                           lastClientIndex(32)
    {
        //         auto serverStartTime = std::chrono::high_resolution_clock::now();
        //do some critical stuff here...
    }

    MapServer::~MapServer()
    {
    }

    int MapServer::Run()
    {
        m_server_alive = true;
        spdlog::info("SERVER ALIVE");
        while (m_server_alive)
        {
            //Tick server
            this->DispatchClientComands();
            //Do other stuff???
            //TODO:ADD SOME DELAY HERE(SUB-TODO:SYNCRONIZATION STUFF MANAGER)???
        }
        return 0;
    }

    void MapServer::Initialize(bool useVanillaCommads)
    {
        spdlog::info("STARTING SERVER....");
        if (useVanillaCommads)
        {
            std::map<uint32_t, std::shared_ptr<MAP::IMapCommand>> defaults = {
                {static_cast<uint32_t>(ServerCommandType::SUBSCRIBE), std::make_shared<MAP::Subscribe>()},
                {static_cast<uint32_t>(ServerCommandType::UNSUBSCRIBE), std::make_shared<MAP::Unsubscribe>()},
                {static_cast<uint32_t>(ServerCommandType::UPSERT), std::make_shared<MAP::Upsert>()},
                {static_cast<uint32_t>(ServerCommandType::GET_ACTIVE_POOLS), std::make_shared<MAP::ActivePools>()},
                {static_cast<uint32_t>(ServerCommandType::SPAWN), std::make_shared<MAP::Spawn>()}};
            this->m_commands_manager.AddSet(defaults);
        }
        m_transport = this->m_current_transport.GetCurrentMethod();
        m_dataFormater = this->m_current_format.GetCurrentMethod();
        m_serverCommands = this->m_commands_manager.GetDefaultSet();
        this->PollMessages();
        spdlog::info("SERVER STARTED....");
    }

    void MapServer::PollMessages()
    {
        //TODO: MIGHT BE MULTI THREAD PROBLEMS WITH uint8_t *bytes IN LAMDA EXPRESSION
        m_transport->PollMessages([&](uint8_t *bytes, std::size_t length)
                                  {
                                      //Decode incoming bytes
                                      auto dataSequence = m_dataFormater->Decode(bytes, length);
                                      //Get sequence command info...
                                      std::shared_ptr<IMapObject> command = dataSequence.at(0);
                                      auto clientId = command->GetInt32("clientId");
                                      auto commandId = command->GetByte("id");
                                      dataSequence.erase(dataSequence.begin()); //CHECK THIS!! (DELETES COMMAND OBJECT FROM SEQUENCE)
                                      MAP::CommandArgs commandArg(GetCommandInfo(clientId), dataSequence);
                                      //Execute
                                      m_serverCommands[commandId]->Evalute(this, commandArg);
                                  });
    }

    int32_t MapServer::ConnectedClients()
    {
        return m_session_clients.size();
    }

    std::shared_ptr<MAP::Client> MapServer::AddClient(const std::shared_ptr<MAP::Client> &newClient)
    {
        m_session_clients.push_back(newClient);
        return m_session_clients.back();
    }

    void MapServer::RemoveClient(int32_t clientId)
    {
    }

    std::shared_ptr<IMapDataFormat> MapServer::GetCurrentDataFormat()
    {
        return m_dataFormater;
    }

    MAP::Vector<std::shared_ptr<MAP::Client>> MapServer::Clients()
    {
        return m_session_clients;
    }

    MAP::Vector<MAP::SpawnedEntity> MapServer::GetPoolSpawnedEntities(uint32_t poolId)
    {
        return m_spawned_entities;
    }

    void MapServer::SpawnObject(int32_t poolId, SpawnedEntity entity)
    {
        ///TODO ADD MUTEX
        m_spawned_entities.push_back(entity);
    }

    uint32_t MapServer::UpsertValue(int32_t poolId, uint32_t key, std::shared_ptr<IMapObject> value)
    {
        //Mutex ????
        return 0;
    }

    void MapServer::PushCommand(uint32_t commandId, bool broadcast, std::vector<std::shared_ptr<MAP::IMapObject>> payLoad, std::shared_ptr<MAP::Client> owner)
    {
        m_command_mutex.lock();
        m_command_queue.push_back(MAP::Command(commandId, payLoad, broadcast, owner));
        m_command_mutex.unlock();
    }

    bool MapServer::ShouldSendData(const MAP::Command &command, std::shared_ptr<MAP::Client> client)
    {
        if (command.IsBroadCast)
            return command.Owner->UserId != client->UserId;
        else
            return command.Owner->UserId == client->UserId;
    }

    void MapServer::DispatchClientComands()
    {
        //TODO: ADD MULTI THREAD DISPATCH... HOLY SHIIIIET
        auto queueCopy = m_command_queue;
        for (const auto &commands : m_command_queue)
        {
            if (!queueCopy.empty())
            {
                auto top = queueCopy.back();
                for (const auto &client : m_session_clients)
                {
                    if (ShouldSendData(top, client))
                    {
                        SendToClient(top, client);
                    }
                }
                queueCopy.pop_back();
            }
        }

        m_command_mutex.lock();
        if (!m_command_queue.empty())
        {
            m_command_queue.pop_back();
        }
        m_command_mutex.unlock();
    }

    //TODO; CASO PARA CUANDO NO EXISTA USUARIO Y IMPLEMENTAR CACHE
    std::shared_ptr<MAP::Client> MapServer::GetCommandInfo(int clientId)
    {
        if (clientId <= 0)
            return nullptr;

        auto client = std::find_if(m_session_clients.begin(), m_session_clients.end(), [&](std::shared_ptr<MAP::Client> e)
                                   {
                                       if (e->UserId == clientId)
                                           return true;
                                       else
                                           return false;
                                   });
        return *client;
    }

    void MapServer::SendToClient(const MAP::Command &command, std::shared_ptr<MAP::Client> client)
    {
        std::vector<std::shared_ptr<MAP::IMapObject>> objStructure =
            {
                m_dataFormater->CreateCommand(command.Code, client->UserId)};

        objStructure.insert(objStructure.begin(), command.PayLoad.begin(), command.PayLoad.end());
        auto memoryBuffer = m_dataFormater->Encode(objStructure);
        m_transport->Send(memoryBuffer.data(), memoryBuffer.size(), client->UserId);
    }
}