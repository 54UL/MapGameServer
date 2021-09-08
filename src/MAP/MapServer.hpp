#ifndef MAP_SERVER_H
#define MAP_SERVER_H
#include <asio.hpp>
#include <cstdlib>
#include <iostream>
#include "Api.hpp"
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
        void MultiThread(asio::io_context &io_context);
        void SingleThread(asio::io_context &io_context);
        void Initialize();
        void TickServer();
        void DispatchClientComands();
        void DecodeCommand(MAP::ServerCommandType header, MAP::CommandArgs &payload);
        void RegisterCommand(MAP::ServerCommandType code, std::function<void(MAP::CommandArgs &payload)> callback);
        void RegisterCommands();
        std::shared_ptr<MAP::Client> GetCommandInfo(int clientId);
        void ReciveData();
        void SendData(uint8_t *charArrayData, std::size_t length, asio::ip::udp::endpoint to);
        bool ShouldSendData(const MAP::Command &command, std::shared_ptr<MAP::Client> client);
        void SendToClient(const MAP::Command &command, std::shared_ptr<MAP::Client> client);

        void OnRecive(const uint8_t *data, std::size_t length);

        //SERVER COMMANDS
        void Subscribe(MAP::CommandArgs &args);
        void Unsubscribe(MAP::CommandArgs &args);
        void StartPool(MAP::CommandArgs &args);
        void EndPool(MAP::CommandArgs &args);
        void UpssertProperty(MAP::CommandArgs &args);
        void RemoveProperty(MAP::CommandArgs &args);
        void SpawnObject(MAP::CommandArgs &args);
        void GetActivePools(MAP::CommandArgs &args);

    private:
        MAP::Vector<std::shared_ptr<MAP::Client>> connectedClients_;
        std::map<MAP::ServerCommandType, std::function<void(MAP::CommandArgs &args)>> commands_;
        MAP::Vector<MAP::Command> commandQueue_;
        MAP::Vector<MAP::SpawnedEntity> spawnedObjects_; //TESTING MUST BE INSIDE OF THE POOL
        udp::socket socket_;
        uint64_t serverTicks_;
        uint64_t lastClientIndex;
        uint8_t data_[MAX_DATA_PAYLOAD];
        std::mutex commandMutex_; // protects all command and data operations
        std::map<std::string, std::string> testingPool_;
        std::mutex consumedTickMutex; // protects all command and data operations
        MAP::Vector<udp::endpoint> alreadyCosumedTickTime_;
        udp::endpoint receiverEndpoint_;
        udp::endpoint lastCommandOwnerEndpoint_;
        std::thread receiverThread_, dispatcherThread_;
    };
}

#endif