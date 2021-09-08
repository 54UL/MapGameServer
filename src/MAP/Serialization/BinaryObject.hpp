#ifndef BINARY_OBJECT_H
#define BINARY_OBJECT_H

#include <stdint.h>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>
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

        template <class T>
        static std::shared_ptr<T> Get(std::map<std::string, std::shared_ptr<MAP::INetworkType>> &sequence, std::string name)
        {
            return std::dynamic_pointer_cast<T>(sequence[name]);
        }

        template <class T>
        static std::shared_ptr<T> Get(NetworkObject &sequence, std::string name)
        {
            auto findedElement = std::find_if(sequence.begin(),sequence.end(),[&](std::shared_ptr<MAP::INetworkType> typeIt){
                return std::strcmp(typeIt->GetName(),name.c_str()) == 0 ;
            });
        
            return std::dynamic_pointer_cast<T>(*findedElement);
        }
    };

    static BinaryObject binaryParser;
}

#endif