#ifndef ACTIVE_POOLS_COMMAND_H
#define ACTIVE_POOLS_COMMAND_H

#include <spdlog/spdlog.h>
#include "../../include/MapServer.hpp"
#include "../../include/MapApi.hpp"

namespace MAP
{
    class ActivePools : public IMapCommand
    {
    public:
        ActivePools() {}
        ~ActivePools() {}

        void Evalute(MapServer *context, CommandArgs &args)
        {
            //AT THE MOMENT EVERYTHING BELOW IS A PLACEHOLDER CODE (JUST TESTING)
            auto dataFormater = context->GetCurrentDataFormat();

            std::vector<std::shared_ptr<MAP::IMapObject>> poolData = {
                dataFormater->CreateObject(666, "PoolId"),
                dataFormater->CreateObject("DEFAULT POOL", "PoolName")};

            std::vector<std::shared_ptr<MAP::IMapObject>>
                commandPayload = {
                    dataFormater->CreateObject(poolData, "poolObj1"),
                    dataFormater->CreateObject(poolData, "poolObj2"),
                    dataFormater->CreateObject(poolData, "poolObj3"),
                    dataFormater->CreateObject(poolData, "poolObj4")};
            context->PushCommand(static_cast<uint8_t>(ServerCommandType::GET_ACTIVE_POOLS), false, commandPayload, args.Owner);
        }

        uint32_t Id() override
        {
            return static_cast<uint32_t>(ServerCommandType::GET_ACTIVE_POOLS);
        }
    };
}

#endif