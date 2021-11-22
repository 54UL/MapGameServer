#ifndef MAP_SERVER_H
#define MAP_SERVER_H

#include <map>
#include <string>
#include <chrono>
#include <thread>
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
        
        //EXPOSED TO COMMANDS API...
        void PushCommand(uint32_t commandId, bool broadcast, std::vector<IMapObject> payLoad, std::shared_ptr<MAP::Client> owner);
    private:
        //Server API
        void MultiThread();
        void SingleThread();
        void Initialize();
        bool ShouldSendData(const MAP::Command &command, std::shared_ptr<MAP::Client> client);
        void SendToClient(const MAP::Command &command, std::shared_ptr<MAP::Client> client);
        std::shared_ptr<MAP::Client> GetCommandInfo(int clientId);
        void OnRecive(const uint8_t *data, std::size_t length);
        void DispatchClientComands();

    private:
        uint64_t serverTicks_;
        uint64_t lastClientIndex;

        MAP::Vector<MAP::Command> commandQueue_;
        MAP::Vector<MAP::SpawnedEntity> spawnedObjects_;
        MAP::Vector<std::shared_ptr<MAP::Client>> connectedClients_;

        std::map<std::string, MAP::NetworkField> testingPool_;
        std::map<MAP::ServerCommandType, std::function<void(MAP::CommandArgs &args)>> commands_;

        //Server threading...
        std::mutex commandMutex_;
        std::thread receiverThread_, dispatcherThread_;

        //Serialization config
        MAP::FormatManager m_current_format;
        MAP::TransportManager m_current_transport;
        MAP::CommandsManager m_commands_manager;
    };
}

#endif