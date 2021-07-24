#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <stdint.h>
#include <memory>
#include <vector>
#include <map>
#include "SerializerAPI.hpp"

namespace MAP
{
    class Serializer
    {
    public:
        Serializer();
        ~Serializer();

    private:
        void Initialize()
        {
            RegisterCoreTypes();
            InitializeMembers();
        }

        inline void RegisterCoreTypes()
        {
        }

        inline void InitializeMembers()
        {
        }

    public:
        std::vector<uint8_t> SerializeDataSequence(std::vector<std::shared_ptr<INetworkType>> sequence)
        {
            //Evalua toda la sequencia y retorna el valor de memoria puro.
        }

        std::vector<std::shared_ptr<INetworkType>> DeserializeDataSequence(const uint8_t *bytes)
        {
            std::vector<std::shared_ptr<INetworkType>> objectStructure;
            auto byteSequenceLength = sizeof(bytes);
            for (uint32_t memPos = 0; memPos < byteSequenceLength;)
            {
                auto memoryValue = bytes[memPos];
                auto typeCode = static_cast<NetworkType>(memoryValue);

                if (memoryValue == 0)
                    return std::vector<std::shared_ptr<INetworkType>>();
                m_current_processing_type = typeCode;

                auto currentDeserializedType = m_serializer_network_types[typeCode]->Deserialize(this);
                // objectStructure.insert(objectStructure.end(), currentDeserializedType);
                memPos = m_current_memory_position;
            }
            return objectStructure;
        }

        //Push all args of an sequence into the memory stack
        bool PushArgs(std::vector<uint8_t> stackValues)
        {
        }

        //Pops all args pushed by an sequence
        std::vector<uint8_t> PopArgs()
        {
            return std::vector<uint8_t>();
        }

        void AddMemoryCounter(uint32_t memoryPosition)
        {
            m_current_memory_position += memoryPosition;
        }

        void DeterminePayLoad(const INetworkType *type)
        {
        }

        //Returns an vector from the current memory position
        std::vector<uint8_t> GetCurrentPayLoad()
        {
            return std::vector<uint8_t>();
        }

    private:
        NetworkType m_current_processing_type;
        uint32_t m_current_memory_position = 0;
        std::vector<uint8_t> m_raw_memory_packet;
        std::vector<std::shared_ptr<INetworkType>> m_current_memory_packet;
        std::map<NetworkType, std::shared_ptr<INetworkType>> m_serializer_network_types;
    };
}

#endif