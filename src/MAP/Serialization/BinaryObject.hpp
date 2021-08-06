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
        BinaryObject()
        {
        }
        ~BinaryObject()
        {
        }

    public:
        uint8_t *Encode(std::vector<std::shared_ptr<INetworkType>> sequence)
        {
            std::vector<uint8_t> m_raw_memory_packet;
            //Evalua toda la sequencia y retorna el valor de memoria puro.
            return &m_raw_memory_packet[0];
        }

        std::vector<std::shared_ptr<INetworkType>> Decode(uint8_t *bytes)
        {
            std::vector<std::shared_ptr<INetworkType>> objectStructure;
            auto byteSequenceLength = sizeof(bytes);
            for (uint32_t memPos = 0; memPos < byteSequenceLength;)
            {
                auto currentDeserializedBytes = 0;
                auto commandValue = bytes[memPos];
                auto typeCode = static_cast<MAP::NetworkType>(commandValue);

                if (commandValue == 0)
                {
                    std::cout << "Missing binary command" << std::endl;
                    return std::vector<std::shared_ptr<INetworkType>>();
                }

                auto currentDeserializedType = m_networkTypes[typeCode]->Deserialize(bytes + memPos);

                for (auto dtype : currentDeserializedType)
                {
                    currentDeserializedBytes += dtype->GetSize();
                }

                objectStructure.insert(objectStructure.end(), currentDeserializedType.begin(), currentDeserializedType.end());
                memPos += currentDeserializedBytes;
            }
            return objectStructure;
        }

    private:
        std::vector<uint8_t> m_raw_memory_packet;
        uint32_t m_current_memory_position = 0;
        std::map<MAP::NetworkType, std::shared_ptr<INetworkType>> m_networkTypes;
    };
}

#endif