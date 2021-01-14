#ifndef MAP_SERVER_H
#define MAP_SERVER_H

#include "json.hpp"
#include <asio.hpp>
#include <cstdlib>
#include <iostream>
#include "Api.hpp"
#include <functional>
#include <map>
#include <string>
#include <chrono>
#include "Models/Client.hpp"
#include "./Models/Command.hpp"
#include "./Models/CommandArgs.hpp"

using asio::ip::udp;
using namespace nlohmann;

#define MAX_DATA_PAYLOAD 512
#define MAX_CLIENTS 16

class MapServer
{
public:
    MapServer(asio::io_context &io_context, short port);
    ~MapServer();

private:
    // MAP CORE
    void Initialize();
    void TickServer();
    void DispatchClientComands();
    void DecodeCommand(std::string header, MAP::CommandArgs &payload);
    std::shared_ptr<MAP::Client> GetCommandInfo(int clientId);
    void ReciveData();
    void SendData(const char *charArrayData, std::size_t length, asio::ip::udp::endpoint to);
    void CreateCommand(const char *name, std::function<void(MAP::CommandArgs &payload)> lamda);
    void RegisterCommands();
    bool ShouldSendData(const MAP::Command &command,   std::shared_ptr<MAP::Client> client);
    void SendToClient(const MAP::Command &command,   std::shared_ptr<MAP::Client> client);
    //SERVER COMMANDS IMPLEMENTATION v1.0(parasite class)
    void Subscribe(MAP::CommandArgs &args);
    void Unsubscribe(MAP::CommandArgs &args);
    void StartPool(MAP::CommandArgs &args);
    void EndPool(MAP::CommandArgs &args);
    void UpssertProperty(MAP::CommandArgs &args);
    void RemoveProperty(MAP::CommandArgs &args);
    void SpawnObject(MAP::CommandArgs &args);
    void GetActivePools(MAP::CommandArgs &args);
    bool IgnoreRequest(udp::endpoint& client);
private:
    MAP::Vector<std::shared_ptr<MAP::Client>> connectedClients_;
    std::map<std::string, std::function<void(MAP::CommandArgs &args)>> commands_;
    MAP::Vector<MAP::Command> commandQueue_;
    MAP::Vector<MAP::SpawnedEntity> spawnedObjects_; //TESTING MUST BE INSIDE OF THE POOL
    udp::socket socket_;
    uint64_t serverTicks_;
    uint64_t lastClientIndex;
    char data_[MAX_DATA_PAYLOAD];
    std::mutex commandMutex_;  // protects all command and data operations
    std::map<std::string,std::string> testingPool_;
    std::mutex consumedTickMutex;  // protects all command and data operations
    MAP::Vector<udp::endpoint> alreadyCosumedTickTime_;
    udp::endpoint receiver_endpoint;
    udp::endpoint lastCommandOwnerEndpoint;
};

#endif