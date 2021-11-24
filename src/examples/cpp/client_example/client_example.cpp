#include <iostream>
#include <future>
#include <thread>

using namespace MAP;

int main(int argc, char *argv[])
{
    // const int port = 5400;

    // MapClient client_example("Client name", port);
    // auto connection = client_example.connect();
    // auto activePools = connection.getActivePools(); // Internal comand send
    // auto current_pool_info = connection.joinPool(somePoolId);
    // connection.send(bytes, length, clientId);

    // //This runs in other thread...

    // client_example.PollServerChanges([](MapClient *context, MapNetworkData data)
    //                                  {
    //                                      //Add some synchronization primitive here (if needded)
    //                                      switch (data.operationType)
    //                                      {
    //                                      case MapOperationType::UPSERT:
    //                                          auto playerPosx = data.Get("posx")->toInt32();
    //                                          auto playerPosy = data.Get("posy")->toInt32();
    //                                          //Add the snapshot to the timeline (ONLY OBJECTS U WANT TO SYNCHRONZE)

    //                                          //  context->TrackNetworkData(data,[](){

    //                                          //  });

    //                                          moveNetworkPlayer(playerPosX, playerPosy);
    //                                          break;

    //                                      case MapOperationType::SPAWN:
    //                                          auto prefabName = data.Get("prefabName")->toString();
    //                                          auto ownerId = data.Get("ownerId")->toInt32();
    //                                          spawnNetworkPlayer(prefabName, ownerId);
    //                                          break;

    //                                      case MapOperationType::REMOVE:
    //                                          auto prefabName = data.Get("prefabName")->toString();
    //                                          auto ownerId = data.Get("ownerId")->toInt32();
    //                                          spawnNetworkPlayer(prefabName, ownerId);
    //                                          break;
    //                                      }
    //                                  });

    return;
}