// #include <mutex>
// #include <thread>
// #include <asio.hpp>

// #include "./MapServer.hpp"
// #include "./Serialization/BinaryUtils.hpp"
// #include "./Serialization/SerializerAPI.hpp"
// #include "./Serialization/Types/Command.hpp"

// // #define SINGLE_THREAD
// namespace MAP
// {
//     using asio::ip::udp;
  
//     MapServer::MapServer(asio::io_context &io_context, short port) : socket_(io_context, udp::endpoint(udp::v4(), port)),
//                                                                      serverTicks_(0),
//                                                                      lastClientIndex(32)
//     {
//         auto serverStartTime = std::chrono::high_resolution_clock::now();
//         // serializer_ = std::make_shared<MAP::BinaryUtils>();

// #ifndef SINGLE_THREAD
//         MultiThread(io_context);
// #else
//         SingleThread(io_context);
// #endif
//         Initialize();
//         std::cout << "SERVER RUNNING..." << std::endl;
//     }

//     MapServer::~MapServer()
//     {
//         receiverThread_.join();
//         dispatcherThread_.join();
//     }

//     void MapServer::MultiThread(asio::io_context &io_context)
//     {
//         std::cout << "MULTI THREAD IMPL..." << std::endl;
//         receiverThread_ = std::thread([&]()
//                                       {
//                                           while (1)
//                                           {
//                                               ReceiveData();
//                                               // std::this_thread::sleep_for(std::chrono::milliseconds(1));
//                                               io_context.run();
//                                               io_context.reset();
//                                               // std::this_thread::sleep_for(std::chrono::milliseconds(14));
//                                           }
//                                       });
//         dispatcherThread_ = std::thread([&]()
//                                         {
//                                             while (1)
//                                             {
//                                                 TickServer();
//                                                 io_context.run();
//                                                 io_context.reset();

//                                                 // auto thread2ServerTime = std::chrono::high_resolution_clock::now();
//                                                 // auto currentTime = thread2ServerTime - serverStartTime;
//                                                 // std::cout << "Trhead 2 server time:" << currentTime.count() << std::endl;
//                                             }
//                                         });
//     }

//     void MapServer::SingleThread(asio::io_context &io_context)
//     {
//         std::cout << "SINGLE THREAD IMPL..." << std::endl;
//         while (1)
//         {
//             io_context.reset();
//             ReceiveData();
//             io_context.run();
//             TickServer();
//             // auto thread1ServerTime = std::chrono::high_resolution_clock::now();
//             // auto currentTime = thread1ServerTime - serverStartTime;
//             // std::cout << "Trhead 1 server time:" << currentTime.count() << std::endl;
//         }
//     }

//     void MapServer::Initialize()
//     {
//         RegisterCommands();
//     }

//     void MapServer::RegisterCommands()
//     {
//         RegisterCommand(MAP::ServerCommandType::SUBSCRIBE, std::bind(&MapServer::Subscribe, this, std::placeholders::_1));
//         RegisterCommand(MAP::ServerCommandType::UNSUBSCRIBE, std::bind(&MapServer::Unsubscribe, this, std::placeholders::_1));
//         RegisterCommand(MAP::ServerCommandType::START_POOL, std::bind(&MapServer::StartPool, this, std::placeholders::_1));
//         RegisterCommand(MAP::ServerCommandType::END_POOL, std::bind(&MapServer::EndPool, this, std::placeholders::_1));
//         RegisterCommand(MAP::ServerCommandType::UPSERT, std::bind(&MapServer::UpssertProperty, this, std::placeholders::_1));
//         RegisterCommand(MAP::ServerCommandType::REMOVE, std::bind(&MapServer::RemoveProperty, this, std::placeholders::_1));
//         RegisterCommand(MAP::ServerCommandType::SPAWN, std::bind(&MapServer::SpawnObject, this, std::placeholders::_1));
//         RegisterCommand(MAP::ServerCommandType::GET_ACTIVE_POOLS, std::bind(&MapServer::GetActivePools, this, std::placeholders::_1));
//     }

//     void MapServer::RegisterCommand(MAP::ServerCommandType code, std::function<void(MAP::CommandArgs &payload)> callback)
//     {
//         commands_.insert(std::make_pair(code, callback));
//     }

//     void MapServer::TickServer()
//     {
//         auto tickStartTime = std::chrono::high_resolution_clock::now();
//         DispatchClientComands();
//         std::this_thread::sleep_for(std::chrono::milliseconds(1));
//         //     auto currentTime = std::chrono::high_resolution_clock::now();
//         //     std::chrono::duration<double, std::milli> duration = currentTime - tickStartTime;
//         //     if (duration.count() <= 8)
//         //     {
//         //         continue;
//         //     }
//         //     else
//         //     {
//         //         serverTicks_++;
//         //         break;
//         //     }
//         // }
//     }

//     void MapServer::DispatchClientComands()
//     {
//         auto queueCopy = commandQueue_;
//         for (const auto &commands : commandQueue_)
//         {
//             if (!queueCopy.empty())
//             {
//                 auto top = queueCopy.back();
//                 for (const auto &client : connectedClients_)
//                 {
//                     if (ShouldSendData(top, client))
//                     {
//                         SendToClient(top, client);
//                     }
//                 }
//                 queueCopy.pop_back();
//             }
//         }

//         commandMutex_.lock();
//         if (!commandQueue_.empty())
//         {
//             commandQueue_.pop_back();
//         }
//         commandMutex_.unlock();
//     }

//     //TODO; CASO PARA CUANDO NO EXISTA USUARIO Y IMPLEMENTAR CACHE
//     std::shared_ptr<MAP::Client> MapServer::GetCommandInfo(int clientId)
//     {
//         if (clientId <= 0)
//             return nullptr;

//         auto client = std::find_if(connectedClients_.begin(), connectedClients_.end(), [&](std::shared_ptr<MAP::Client> e)
//                                    {
//                                        if (e->UserId == clientId)
//                                            return true;
//                                        else
//                                            return false;
//                                    });
//         return *client;
//     }

//     void MapServer::OnRecive(const uint8_t *data, std::size_t length)
//     {
//         std::cout << "[INCOMIG DATA: " << length << " bytes from " << receiverEndpoint_.address() << "] " << std::endl;
//         std::cout << std::hex << data << std::endl; //BINARY ???

//         auto dataSequence = BinaryUtils::DecodeAsMap(data_, length);
//         auto decodedCommand = std::dynamic_pointer_cast<MAP::NetCommand>((dataSequence.at(0)));
//         dataSequence.erase(dataSequence.begin()); //OJO
//         MAP::CommandArgs commandArg(GetCommandInfo(decodedCommand->clientId()), dataSequence);
//         DecodeCommand(static_cast<MAP::ServerCommandType>(decodedCommand->id()), commandArg);
//     }

//     void MapServer::ReceiveData()
//     {
//         socket_.async_receive_from(
//             asio::buffer(data_, MAX_DATA_PAYLOAD), receiverEndpoint_,
//             [&](std::error_code ec, std::size_t bytes_recvd)
//             {
//                 if (!ec && bytes_recvd > 0)
//                 {
//                     try
//                     {
//                         OnRecive(data_, bytes_recvd);
//                     }
//                     catch (std::exception &e)
//                     {
//                         std::cerr << "[COMMAND LOST DUE:  :" << e.what() << "]\n";
//                     }
//                 }
//             });
//     }

//     bool MapServer::ShouldSendData(const MAP::Command &command, std::shared_ptr<MAP::Client> client)
//     {
//         if (command.IsBroadCast)
//             return command.Owner->UserId != client->UserId;
//         else
//             return command.Owner->UserId == client->UserId;
//     }

//     void MapServer::DecodeCommand(MAP::ServerCommandType code, MAP::CommandArgs &payload)
//     {
//         commands_[code](payload);
//     }

//     void MapServer::SendToClient(const MAP::Command &command, std::shared_ptr<MAP::Client> client)
//     {
//         NetworkObject objStructure;
//         objStructure.push_back(std::make_shared<MAP::NetCommand>(command.Code, client->UserId));
//         objStructure.insert(objStructure.begin(), command.PayLoad.begin(), command.PayLoad.end());
//         std::vector<uint8_t> memoryBuffer = BinaryUtils::Encode(objStructure);
//         SendData(memoryBuffer.data(), memoryBuffer.size(), client->ClientEndpoint);
//     }

//     //TODO: VERIFICAR ENVIOS DE CADA CLIENTE (ASEGURAR TICKS SINCRONIZADOS)
//     void MapServer::SendData(uint8_t *charArrayData, std::size_t length, asio::ip::udp::endpoint to)
//     {
//         socket_.async_send_to(
//             asio::buffer(charArrayData, length), to,
//             [this](std::error_code err, std::size_t sended)
//             {
//                 //DO SOMETHING WITH THE THINGS SENDED
//                 //  std::cout << "[DATA SENDED (" << sended << ")]" << std::endl;
//             });
//     }

//     //COMAND IMPLEMENTATION
//     void MapServer::Subscribe(MAP::CommandArgs &args)
//     {
//         std::string IpAddress = BinaryUtils::Get<MAP::NetString>(args.Payload, "IpAddress")->GetValue();
//         std::string playerName = BinaryUtils::Get<MAP::NetString>(args.Payload, "PlayerName")->GetValue();
//         std::string hostName = BinaryUtils::Get<MAP::NetString>(args.Payload, "HostName")->GetValue();
//         int port = BinaryUtils::Get<MAP::NetInt>(args.Payload, "Port")->GetValue();

//         if (IpAddress.compare("") == 0)
//             return;

//         if (connectedClients_.size() < MAX_CLIENTS)
//         {
//             auto userId = lastClientIndex++;
//             auto ipAddrr = asio::ip::address::from_string(IpAddress);
//             auto clientEndpoint = udp::endpoint(ipAddrr, port);
//             auto newClient = std::make_shared<MAP::Client>(userId, playerName, hostName, clientEndpoint, port);
//             connectedClients_.emplace_back(newClient);

//             //SEND REPLY
//             NetworkObject commandPayload;
//             NetworkObject spawnedObjs;

//             for (const auto &entity : spawnedObjects_)
//             {
//                 NetworkObject entityStructure{
//                     std::make_shared<MAP::NetString>(entity.PrefabName, "PrefabName"),
//                     std::make_shared<MAP::NetInt>(entity.PlayerOwner, "PlayerId"),
//                 };

//                 spawnedObjs.insert(spawnedObjs.end(), entityStructure.begin(), entityStructure.end());
//             }

//             commandPayload.push_back(std::make_shared<MAP::NetInt>(userId, "ClientId"));
//             commandPayload.push_back(std::make_shared<MAP::NetString>("null", "AccesToken"));
//             commandPayload.push_back(std::make_shared<MAP::NetArray>(spawnedObjs, "SpawnedEntities"));
//             std::cout << "user conected" << std::endl;
//             commandMutex_.lock();
//             commandQueue_.push_back(MAP::Command(static_cast<uint8_t>(ServerCommandType::SUBSCRIBE), commandPayload, false, connectedClients_.back()));
//             commandMutex_.unlock();
//         }
//     }

//     void MapServer::GetActivePools(MAP::CommandArgs &args)
//     {
//         //test elements
//         NetworkObject poolData{
//             std::make_shared<MAP::NetInt>(666, "PoolId"),
//             std::make_shared<MAP::NetString>("DEFAULT POOL", "PoolName")};

//         NetworkObject
//             commandPayload{
//                 std::make_shared<MAP::NetArray>(poolData, "PoolObj1"),
//                 std::make_shared<MAP::NetArray>(poolData, "PoolObj2"),
//                 std::make_shared<MAP::NetArray>(poolData, "PoolObj3"),
//                 std::make_shared<MAP::NetArray>(poolData, "PoolObj4")};

//         commandMutex_.lock();
//         commandQueue_.push_back(MAP::Command(static_cast<uint8_t>(ServerCommandType::GET_ACTIVE_POOLS), commandPayload, false, args.Owner));
//         commandMutex_.unlock();
//     }

//     void MapServer::Unsubscribe(MAP::CommandArgs &args)
//     {
//     }

//     void MapServer::StartPool(MAP::CommandArgs &args)
//     {
//     }

//     void MapServer::EndPool(MAP::CommandArgs &args)
//     {
//     }

//     void MapServer::UpssertProperty(MAP::CommandArgs &args)
//     {
//         std::string payloadKey = BinaryUtils::Get<MAP::NetString>(args.Payload, "Key")->GetValue();
//         auto payloadValue = args.Payload["Value"];
//         //Set internal data
//         testingPool_[payloadKey] = payloadValue;
//         //Broadcast payload
//         NetworkObject commandPayload{
//             std::make_shared<MAP::NetString>(payloadKey, "Key"),
//             payloadValue};

//         commandMutex_.lock();
//         commandQueue_.push_back(MAP::Command(static_cast<uint8_t>(ServerCommandType::UPSERT), commandPayload, true, args.Owner));
//         commandMutex_.unlock();
//     }

//     void MapServer::RemoveProperty(MAP::CommandArgs &args)
//     {
//     }

//     void MapServer::SpawnObject(MAP::CommandArgs &args)
//     {
//         int playerId = BinaryUtils::Get<MAP::NetInt>(args.Payload, "PlayerId")->GetValue();
//         std::string prefabName = BinaryUtils::Get<MAP::NetString>(args.Payload, "PrefabName")->GetValue();
//         MAP::SpawnedEntity spawnedEntity(prefabName, playerId);
//         NetworkObject commandPayload{
//             args.Payload["PlayerId"],
//             args.Payload["PrefabName"]
//             };
//         commandMutex_.lock();
//         spawnedObjects_.push_back(spawnedEntity);
//         commandQueue_.push_back(MAP::Command(static_cast<uint8_t>(ServerCommandType::SPAWN), commandPayload, true, args.Owner));
//         commandMutex_.unlock();
//     }
// }