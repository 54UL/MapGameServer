#ifndef BINARY_OBJECT_H
#define BINARY_OBJECT_H

#include <stdint.h>
#include <memory>
#include <vector>
#include <map>
#include "./SerializerAPI.hpp"
#include "./TypesManager.hpp"
#include <iostream>

namespace MAP
{
    class BinaryObject
    {

    public:
        BinaryObject();
        ~BinaryObject();

    public:
        std::vector<uint8_t> Encode(std::vector<std::shared_ptr<INetworkType>> sequence);
        std::map<std::string, std::shared_ptr<INetworkType>> DecodeAsMap(uint8_t *bytes, std::size_t length);
        std::vector<std::shared_ptr<INetworkType>> Decode(std::vector<uint8_t> bytes);
    };

    static BinaryObject binaryParser;
}

#endif