#include "MapServer.hpp"
#include <taskflow.hpp>
#include <asio.hpp>
#include <mutex>

// #define SINGLE_THREAD

using namespace nlohmann;
using asio::ip::udp;

//TODO: REFACTOR AL CONSTRUCTOR
MapServer::MapServer(asio::io_context &io_context, short port)
    : socket_(io_context, udp::endpoint(udp::v4(), port)), serverTicks_(0), lastClientIndex(32)
{
    auto serverStartTime = std::chrono::high_resolution_clock::now();
#ifndef SINGLE_THREAD
    std::cout << "MULTI THREAD IMPL..." << std::endl;
    tf::Executor executor;
    tf::Taskflow mainTask;
    mainTask.emplace(
        [&]() {
            while (1)
            {
                ReciveData();
                // std::this_thread::sleep_for(std::chrono::milliseconds(1));
                io_context.run();
                io_context.reset();
                // std::this_thread::sleep_for(std::chrono::milliseconds(14));
            }
        },
        [&]() {
            while (1)
            {
                TickServer();
                io_context.run();
                io_context.reset();

                // auto thread2ServerTime = std::chrono::high_resolution_clock::now();
                // auto currentTime = thread2ServerTime - serverStartTime;
                // std::cout << "Trhead 2 server time:" << currentTime.count() << std::endl;
            }
        },
        [&]() {
            while (1)
            {
                consumedTickMutex.lock();
                auto size = alreadyCosumedTickTime_.size();
                if (size > 0)
                {
                    alreadyCosumedTickTime_.clear();
                    // std::cout << "CLEARED ALREADY CONSUMED TICK USER LIST size:" << size << std::endl;
                }
                consumedTickMutex.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(8));
            }
        });
    mainTask.emplace(
        [&]() {
            while (1)
            {
                consumedTickMutex.lock();
                auto size = alreadyCosumedTickTime_.size();
                if (size > 0)
                {
                    alreadyCosumedTickTime_.clear();
                    std::cout << "CLEARED ALREADY CONSUMED TICK USER LIST size:" << size << std::endl;
                }
                consumedTickMutex.unlock();
                std::this_thread::sleep_for(std::chrono::milliseconds(8));
            }
        });
#endif
    Initialize();
#ifndef SINGLE_THREAD
    executor.run(mainTask);
#endif
    std::cout << "SERVER RUNNING..." << std::endl;
#if defined(SINGLE_THREAD) 
    std::cout << "SINGLE THREAD IMPL..." << std::endl;
    while (1)
    {
        io_context.reset();
        ReciveData();
        io_context.run();
        TickServer();
        // auto thread1ServerTime = std::chrono::high_resolution_clock::now();
        // auto currentTime = thread1ServerTime - serverStartTime;
        // std::cout << "Trhead 1 server time:" << currentTime.count() << std::endl;
    }
#endif
}

MapServer::~MapServer()
{
}

void MapServer::Initialize()
{
    RegisterCommands();
    json defaultPoolObj;
    defaultPoolObj["PoolName"] = "DEFAULT POOL,WELCOME";
    //StartPool(MAP::CommandArgs(std::make_shared<MAP::Client>(), defaultPoolObj));
}

void MapServer::RegisterCommands()
{
    CreateCommand("SUBSCRIBE", std::bind(&MapServer::Subscribe, this, std::placeholders::_1));
    CreateCommand("UNSUBSCRIBE", std::bind(&MapServer::Unsubscribe, this, std::placeholders::_1));
    CreateCommand("START_POOL", std::bind(&MapServer::StartPool, this, std::placeholders::_1));
    CreateCommand("END_POOL", std::bind(&MapServer::EndPool, this, std::placeholders::_1));
    CreateCommand("UPSERT", std::bind(&MapServer::UpssertProperty, this, std::placeholders::_1));
    CreateCommand("REMOVE", std::bind(&MapServer::RemoveProperty, this, std::placeholders::_1));
    CreateCommand("SPAWN", std::bind(&MapServer::SpawnObject, this, std::placeholders::_1));
    CreateCommand("GET_ACTIVE_POOLS", std::bind(&MapServer::GetActivePools, this, std::placeholders::_1));
}

void MapServer::CreateCommand(const char *name, std::function<void(MAP::CommandArgs &payload)> lamda)
{
    commands_.insert(std::make_pair(name, lamda));
}

void MapServer::TickServer()
{
    auto tickStartTime = std::chrono::high_resolution_clock::now();
    DispatchClientComands();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
   
    //     auto currentTime = std::chrono::high_resolution_clock::now();
    //     std::chrono::duration<double, std::milli> duration = currentTime - tickStartTime;
    //     if (duration.count() <= 8)
    //     {
    //         continue;
    //     }
    //     else
    //     {
    //         serverTicks_++;
    //         break;
    //     }
    // }
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

//TODO; CASO PARA CUANDO NO EXISTA USUARIO
std::shared_ptr<MAP::Client> MapServer::GetCommandInfo(int clientId)
{
    if (clientId <= 0)
        return nullptr;

    auto client = std::find_if(connectedClients_.begin(), connectedClients_.end(), [&](std::shared_ptr<MAP::Client> e) {
        if (e->UserId == clientId)
            return true;
        else
            return false;
    });
    return *client;
}

bool MapServer::IgnoreRequest(udp::endpoint &client)
{
    if (lastCommandOwnerEndpoint.address() == client.address() && lastCommandOwnerEndpoint.port() == client.port())
    {
        // std::cout << "[IGNORED COMMAND]" << std::endl;
        return true;
    }
    consumedTickMutex.lock();
    auto findedClient = std::find_if(alreadyCosumedTickTime_.begin(), alreadyCosumedTickTime_.end(), [&](udp::endpoint e) {
        if (e.address() == client.address() && e.port() == client.port())
        {
            return true;
        }
        else
        {
            return false;
        }
    });
    if (findedClient != std::end(alreadyCosumedTickTime_))
    {
        consumedTickMutex.unlock();
        // std::cout << "[IGNORED COMMAND]" << std::endl;
        return true;
    }
    else
    {
        alreadyCosumedTickTime_.push_back(client);
        consumedTickMutex.unlock();
        lastCommandOwnerEndpoint = client;
        return false;
    }
}

void MapServer::ReciveData()
{
    socket_.async_receive_from(
        asio::buffer(data_, MAX_DATA_PAYLOAD), receiver_endpoint,
        [&](std::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0)
            {
                try
                {
                    //  std::cout << "[INCOMIG DATA: " << bytes_recvd << " bytes from " << receiver_endpoint.address() << "] " << std::endl;
                    data_[bytes_recvd] = '\0';
                    // std::cout << data_ << std::endl;
                    auto stringToBeParsed = std::string(data_).substr(0, bytes_recvd);
                    json commandObj = json::parse(stringToBeParsed.begin(), stringToBeParsed.end());
                    auto header = commandObj["header"].get<std::string>();
                    auto args = json::parse(commandObj["payload"].get<std::string>());
                    auto clientId = commandObj["clientId"].get<int>();
                    //PASAR CODIGO GET COMMAND INFO PARA LA INFORMACION DEL OWNER
                    MAP::CommandArgs commandArg(GetCommandInfo(clientId), args);
                    DecodeCommand(header, commandArg);
                }
                catch (std::exception &e)
                {
                    // std::cerr << "[COMMAND LOST DUE:  :" << e.what() << "]\n";
                }
            }
        });
}

bool MapServer::ShouldSendData(const MAP::Command &command, std::shared_ptr<MAP::Client> client)
{
    if (command.IsBroadCast)
        return command.Owner->UserId != client->UserId;
    else
        return command.Owner->UserId == client->UserId;
}

void MapServer::DecodeCommand(std::string header, MAP::CommandArgs &payload)
{
    commands_[header](payload);
}

void MapServer::SendToClient(const MAP::Command &command, std::shared_ptr<MAP::Client> client)
{
    // json commandObj;
    // commandObj["header"] = command.Header;
    // commandObj["payload"] = command.PayLoad.dump();
    // commandObj["clientId"] = 0;//UNUSED BUT DEFINED IN THE TYPE
    // auto dataString = commandObj.dump(-1, ' ', true);
    // SendData(dataString.c_str(), dataString.length(), client->ClientEndpoint);
}

//TODO: VERIFICAR ENVIOS DE CADA CLIENTE (ASEGURAR TICKS SINCRONIZADOS)
void MapServer::SendData(const char *charArrayData, std::size_t length, asio::ip::udp::endpoint to)
{
    socket_.async_send_to(
        asio::buffer(charArrayData, length), to,
        [this](std::error_code err, std::size_t sended) {
            //DO SOMETHING WITH THE THINGS SENDED
            //  std::cout << "[DATA SENDED (" << sended << ")]" << std::endl;
        });
}

//COMAND IMPLEMENTATION (CONSIDER MOVING THIS TO OTHER CLASSES)
void MapServer::Subscribe(MAP::CommandArgs &args)
{
    // json commandArg;

    // if (args.data["IpAddress"] == "")
    //     return;

    // if (connectedClients_.size() < MAX_CLIENTS)
    // {
    //     auto userId = lastClientIndex++;
    //     auto ipAdrres = asio::ip::address::from_string(args.data["IpAddress"].get<std::string>());
    //     auto clientEndpoint = udp::endpoint(ipAdrres, args.data["Port"]);
    //     auto playerName = args.data["PlayerName"].get<std::string>();
    //     auto hostName = args.data["HostName"].get<std::string>();
    //     auto port = args.data["Port"].get<uint32_t>();
    //     auto newClient = std::make_shared<MAP::Client>(userId, playerName, hostName, clientEndpoint, port);
    //     connectedClients_.emplace_back(newClient);
    //     //SEND REPLY
    //     json commandPayload;
    //     json spawnedObjs;
    //     for (const auto &entity : spawnedObjects_)
    //     {
    //         json entityJson;
    //         entityJson["PrefabName"] = entity.PrefabName;
    //         entityJson["PlayerId"] = entity.PlayerOwner;
    //         spawnedObjs.push_back(entityJson);
    //     }
    //     commandPayload["ClientId"] = userId;
    //     commandPayload["AccesToken"] = "null";
    //     commandPayload["SpawnedEntities"] = spawnedObjs;
    //     std::cout<<"user conected"<<std::endl;
    //     commandMutex_.lock();
    //     commandQueue_.push_back(MAP::Command("CLIENT_INFO", commandPayload, false, connectedClients_.back()));
    //     commandMutex_.unlock();
    // }
}

void MapServer::GetActivePools(MAP::CommandArgs &args)
{
    // json commandPayload;
    // // for(const auto & pools,)
    // // commandQueue_.push_back(MAP::Command("GET_POOLS", commandPayload, false, std::move(lastClient)));
    // json PoolPair;
    // PoolPair["PoolId"] = "666";
    // PoolPair["PoolName"] = "DEFAULT POOL";
    // commandPayload["pools"].push_back(PoolPair);
    // commandPayload["pools"].push_back(PoolPair);
    // commandPayload["pools"].push_back(PoolPair);
    // commandMutex_.lock();
    // commandQueue_.push_back(MAP::Command("GET_POOLS", commandPayload, false, args.Owner));
    // commandMutex_.unlock();
}

void MapServer::Unsubscribe(MAP::CommandArgs &args)
{
}

void MapServer::StartPool(MAP::CommandArgs &args)
{
}

void MapServer::EndPool(MAP::CommandArgs &args)
{
}

void MapServer::UpssertProperty(MAP::CommandArgs &args)
{
    // json commandPayload;
    // testingPool_[args.data["Key"]] = args.data["Value"].get<std::string>();
    // commandPayload["Key"] = args.data["Key"];
    // commandPayload["Value"] = args.data["Value"].get<std::string>();
    // commandMutex_.lock();
    // commandQueue_.push_back(MAP::Command("UPPSERT", commandPayload, true, args.Owner));
    // commandMutex_.unlock();
}

void MapServer::RemoveProperty(MAP::CommandArgs &args)
{
}

void MapServer::SpawnObject(MAP::CommandArgs &args)
{
    // json commandPayload;
    // commandPayload["PrefabName"] = args.data["PrefabName"];
    // commandPayload["PlayerId"] = args.data["PlayerId"];
    // auto owner = args.data["PlayerId"].get<uint64_t>();
    // auto prefabName = args.data["PrefabName"].get<std::string>();
    // MAP::SpawnedEntity spawnedEntity(prefabName,owner);
    // commandMutex_.lock();
    // spawnedObjects_.push_back(spawnedEntity);
    // commandQueue_.push_back(MAP::Command("SPAWN", commandPayload, true, args.Owner));
    // commandMutex_.unlock();
}
