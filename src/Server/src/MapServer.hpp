#ifndef MAP_SERVER_H
#define MAP_SERVER_H
#include <asio.hpp>

#include <cstdlib>
#include <iostream>
#include "../include/MapApi.hpp"
#include <functional>
#include <map>
#include <string>
#include <chrono>
#include "./Models/Client.hpp"
#include "./Models/Command.hpp"
#include "./Models/CommandArgs.hpp"
#include <thread>

// #define SINGLE_THREAD

using asio::ip::udp;

namespace MAP
{
    class MapServer
    {
    public:
        MapServer(asio::io_context &io_context, short port);
        ~MapServer();
    
    private:
        //Server API
        void MultiThread(asio::io_context &io_context);
        void SingleThread(asio::io_context &io_context);
        void Initialize();
        void DispatchClientComands();
        std::shared_ptr<MAP::Client> GetCommandInfo(int clientId);
        void PushCommand();
        void PopCommand();

        //Transport API
        void TickServer();
        void ReceiveData();
        void DecodeCommand(MAP::ServerCommandType header, MAP::CommandArgs &payload);
        void SendData(uint8_t *charArrayData, std::size_t length, asio::ip::udp::endpoint to);
        bool ShouldSendData(const MAP::Command &command, std::shared_ptr<MAP::Client> client);
        void SendToClient(const MAP::Command &command, std::shared_ptr<MAP::Client> client);
        void OnRecive(const uint8_t *data, std::size_t length);
        
        //COMAND API
        void RegisterCommand(MAP::ServerCommandType code, std::function<void(MAP::CommandArgs &payload)> callback);
        void RegisterCommands();
        //SERVER COMMANDS
        void Subscribe(MAP::CommandArgs &args);
        void Unsubscribe(MAP::CommandArgs &args);
        void StartPool(MAP::CommandArgs &args);
        void EndPool(MAP::CommandArgs &args);
        void UpsertProperty(MAP::CommandArgs &args);
        void RemoveProperty(MAP::CommandArgs &args);
        void SpawnObject(MAP::CommandArgs &args);
        void GetActivePools(MAP::CommandArgs &args);

    private:
        uint64_t serverTicks_;
        uint64_t lastClientIndex;
        uint8_t data_[MAX_DATA_PAYLOAD];

        udp::socket socket_;
        udp::endpoint receiverEndpoint_;
        udp::endpoint lastCommandOwnerEndpoint_;
        
        MAP::Vector<MAP::Command> commandQueue_;
        MAP::Vector<MAP::SpawnedEntity> spawnedObjects_; 
        MAP::Vector<std::shared_ptr<MAP::Client>> connectedClients_;
            
        std::map<std::string, MAP::NetworkField> testingPool_;
        std::map<MAP::ServerCommandType, std::function<void(MAP::CommandArgs &args)>> commands_;
        
        //Threading...
        std::mutex commandMutex_;  
        std::thread receiverThread_, dispatcherThread_;
    };
}

#endif