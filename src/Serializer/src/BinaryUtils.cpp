#include "BinaryUtils.hpp"

//TODO: REFACTOR DECODES...
namespace MAP::BinaryUtils
{
    std::vector<uint8_t> Encode(std::vector<std::shared_ptr<INetworkType>> sequence)
    {
        std::vector<uint8_t> m_raw_memory_packet;
        //Evalua toda la sequencia y retorna el valor de memoria puro.
        for (auto typeInstance : sequence)
        {
            auto serializedDataVector = typeInstance->Serialize();
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
            
            auto currentDeserializedType = SerializerTypes.Get()[typeCode]->Deserialize(bytes + memPos);

            for (auto dtype : currentDeserializedType)
            {
                objectStructure.insert(std::make_pair(dtype->GetName(), dtype));
                currentDeserializedBytes += dtype->GetSize();
            }
            memPos += currentDeserializedBytes;
        }
        return objectStructure;
    }

    std::vector<std::shared_ptr<INetworkType>> Decode(std::vector<uint8_t> bytes)
    {
        std::vector<std::shared_ptr<INetworkType>> objectStructure;
        auto byteSequenceLength = bytes.size();

        for (uint32_t memPos = 0; memPos < byteSequenceLength;)
        {
            auto commandValue = bytes.at(memPos);
            auto typeCode = static_cast<MAP::NetworkType>(commandValue);
            auto currentDeserializedBytes = 0;

            auto currentDeserializedType = SerializerTypes.Get()[typeCode]->Deserialize(&bytes.at(memPos));
            for (auto dtype : currentDeserializedType)
            {
                currentDeserializedBytes += dtype->GetSize();
            }
            objectStructure.insert(objectStructure.end(), currentDeserializedType.begin(), currentDeserializedType.end());
            memPos += currentDeserializedBytes;
        }
        return objectStructure;
    }
}