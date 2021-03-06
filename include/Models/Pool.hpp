#ifndef POOL_H
#define POOL_H
#include <string>
#include "../json.hpp"
#include <string>
#include "Client.hpp"
#include "../Api.hpp"
#include "../json.hpp"
using json = nlohmann::json;

namespace MAP
{
    class Pool
    {
    public:
        Pool(std::string name,uint64_t poolId): Name(name), id(poolId)
        {
        }
        uint64_t id;
        std::string Name;
        std::map<std::string, json> dataMap;
    };
} // namespace MAP

#endif