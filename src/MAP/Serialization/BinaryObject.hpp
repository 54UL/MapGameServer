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
        std::vector<uint8_t> Encode(std::vector<std::shared_ptr<INetworkType>> sequence)
        {
            std::vector<uint8_t> m_raw_memory_packet;
            //Evalua toda la sequencia y retorna el valor de memoria puro.
            for (auto typeInstance : sequence)
            {
                auto serializedDataVector = typeInstance->TrySerialize();
                m_raw_memory_packet.insert(m_raw_memory_packet.end(), serializedDataVector.begin(), serializedDataVector.end());
            }
            return m_raw_memory_packet;
        }

        std::map<std::string, std::shared_ptr<INetworkType>> DecodeAsMap(uint8_t *bytes, std::size_t length)
        {
            std::map<std::string, std::shared_ptr<INetworkType>> objectStructure;
            for (uint32_t memPos = 0; memPos < length;)
            {
                auto currentDeserializedBytes = 0;

                auto commandValue = bytes[memPos];
                auto typeCode = static_cast<MAP::NetworkType>(commandValue);

                if (commandValue == 0)
                {
                    std::cout << "Missing binary command" << std::endl;
                    return std::map<std::string, std::shared_ptr<INetworkType>>();
                }

                auto currentDeserializedType = m_network_types.Get()[typeCode]->Deserialize(bytes + memPos);

                for (auto dtype : currentDeserializedType)
                {
                    objectStructure.insert(std::make_pair(dtype->GetName(), dtype));
                    currentDeserializedBytes += dtype->GetSize();
                }
                memPos += currentDeserializedBytes + 1;
            }
            return objectStructure;
        }

        std::vector<std::shared_ptr<INetworkType>> Decode(std::vector<uint8_t> bytes)
        {
            std::vector<std::shared_ptr<INetworkType>> objectStructure;
            auto byteSequenceLength = bytes.size();
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
                auto currentDeserializedType = m_network_types.Get()[typeCode]->Deserialize(&bytes.at(memPos));
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
        MAP::TypesManager m_network_types;
    };
}

#endif