#ifndef MAP_SERVER_H
#define MAP_SERVER_H

#include <map>
#include <string>
#include <chrono>
#include <thread>
#include <memory>
#include <cstdlib>

#include <iostream>
#include <functional>

#include "./MapApi.hpp"
#include "./Models/Client.hpp"
#include "./Models/Command.hpp"
#include "./Models/CommandArgs.hpp"
#include "./FormatManager.hpp"
#include "./TransportManager.hpp"
#include "./CommandsManager.hpp"

// #define SINGLE_THREAD

namespace MAP
{
    class MapServer
    {
    public:
        MapServer(EncodingMethod encoding, TransportMethod transport, bool useDefaultCommands, short port);
        ~MapServer();

        //SERVER COMMAND API
        MAP::Vector<MAP::SpawnedEntity> GetPoolSpawnedEntities(uint32_t poolId);
        int32_t ConnectedClients();
        MAP::Vector<std::shared_ptr<MAP::Client>> Clients();
        void PushCommand(uint32_t commandId, bool broadcast, std::vector<std::shared_ptr<MAP::IMapObject>> payLoad, std::shared_ptr<MAP::Client> owner);
        std::shared_ptr<MAP::Client> AddClient(const std::shared_ptr<MAP::Client> &newClient);
        void RemoveClient(int32_t clientId);
        std::shared_ptr<IMapDataFormat> GetCurrentDataFormat();
        void SpawnObject(int32_t poolId, SpawnedEntity entity);
        uint32_t UpsertValue(int32_t poolId, uint32_t key, std::shared_ptr<IMapObject> value);
        //SERVER COMMAND API END

        //LEGIT PUBLIC MEMBERS
        int Run();//TODO: THIS CAN'T BE RUN INSIDE OF A COMMAND 
        void Initialize(bool useDefaultCommands);//TODO: THIS CAN'T BE RUN INSIDE OF A COMMAND

    private:
        void MultiThread();
        void SingleThread();
        bool ShouldSendData(const MAP::Command &command, std::shared_ptr<MAP::Client> client);
        void SendToClient(const MAP::Command &command, std::shared_ptr<MAP::Client> client);
        std::shared_ptr<MAP::Client> GetCommandInfo(int clientId);
        void PollMessages();
        void DispatchClientComands();

    private:
        bool m_server_alive=false;
        //Map server internal systems...
        MAP::FormatManager m_current_format;
        MAP::TransportManager m_current_transport;
        MAP::CommandsManager m_commands_manager;
        //SYSTEM TODOS
        //server pools manager
        //Thread Pool manager

        //Cached implementations
        std::shared_ptr<MAP::IMapTransport> m_transport;
        std::shared_ptr<MAP::IMapDataFormat> m_dataFormater;
        MAP::Map<uint32_t, std::shared_ptr<MAP::IMapCommand>> m_serverCommands;

        uint64_t serverTicks_;
        uint64_t lastClientIndex;

        MAP::Vector<MAP::Command> m_command_queue;
        MAP::Vector<MAP::SpawnedEntity> m_spawned_entities;
        MAP::Vector<std::shared_ptr<MAP::Client>> m_session_clients;

        std::map<std::string, std::shared_ptr<MAP::IMapObject>> m_testing_pool;
        std::map<MAP::ServerCommandType, std::function<void(MAP::CommandArgs &args)>> commands_;
        //Server threading...
        std::mutex m_command_mutex;
        std::thread m_receiver_thread, m_dispatcher_thread;
    };
}

#endif