
#ifndef NET_ARRAY_TYPE
#define NET_ARRAY_TYPE

#include "../SerializerAPI.hpp"
#include <string>

namespace MAP
{
    class NetArray : public INetworkType
    {
    public:
        NetArray() : m_instance_name("NONE-ARRAY")
        {
        }
        NetArray(std::vector<std::shared_ptr<MAP::INetworkType>> sequence, std::string name) : m_instance_name(name), m_values(sequence)
        {
        }
        virtual ~NetArray()
        {
        }

        std::vector<uint8_t> TrySerialize() override
        {

            std::vector<uint8_t> memoryVector;
            uint8_t sizeInByes = 0;

            memoryVector.push_back((uint8_t)GetType()); //TYPE
            auto memoryTagVector = m_instance_name.TrySerialize().at(0);
            memoryVector.push_back(memoryTagVector); //MEMORY_TAG

            for (auto &arrayVal : m_values)
                sizeInByes = arrayVal->GetSize();

            memoryVector.push_back(sizeInByes); //LENGTH  IN BYTES
            for (auto arrayValue : m_values)
            { //ARRAY_BINARY_VALUE/S
                auto memoryChunk = arrayValue->TrySerialize();
                memoryVector.insert(memoryVector.end(), memoryChunk.begin(), memoryChunk.end());
            }
            return memoryVector;
        }

        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override
        {
            std::vector<std::shared_ptr<INetworkType>> objectStructure;
            auto memoryTag = m_instance_name.Deserialize(argsMemory).at(0);
            auto memoryTagOffset = memoryTag->GetSize();
            auto arrayLength = argsMemory[memoryTagOffset + MEM_OFFSET_1];
            BinaryObject binObj; //HACER DECODE STATIC PARA QUITAR ESTO DE AQUI
            auto valuesMemoryVector = std::vector<uint8_t>(argsMemory+memoryTagOffset+MEM_OFFSET_2, argsMemory+arrayLength);
            auto decodedArrayVal = binObj.Decode(valuesMemoryVector);
            objectStructure.push_back(std::make_shared<MAP::NetArray>(decodedArrayVal, memoryTag->GetName()));
            return objectStructure;
        }

        NetworkType GetType() override
        {
            return NetworkType::ARRAY;
        }

        const char *GetName() override
        {
            return m_instance_name.GetName();
        }

        uint32_t GetSize() override
        {
            std::size_t partialSize = 0;
            for (auto arrayVal : m_values)
                partialSize += arrayVal->GetSize();
            return m_instance_name.GetSize() + partialSize;
        }

        std::vector<std::shared_ptr<MAP::INetworkType>> GetValues()
        {
            return m_values;
        }

    private:
        std::vector<std::shared_ptr<MAP::INetworkType>> m_values;
        MAP::MemoryTag m_instance_name;
    };
}

#endif