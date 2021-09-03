#include "Array.hpp"
#include "../BinaryObject.hpp"
namespace MAP
{
    NetArray::NetArray() : m_instance_name("SYSTEM-ARRAY")
    {
    }

    NetArray::NetArray(std::vector<std::shared_ptr<MAP::INetworkType>> sequence, std::string name) : m_instance_name(name), m_values(sequence)
    {
    }

    NetArray::~NetArray()
    {
    }

    std::vector<uint8_t> NetArray::TrySerialize()
    {
        std::vector<uint8_t> memoryVector;

        memoryVector.push_back((uint8_t)GetType()); //TYPE
        auto memoryTagVector = m_instance_name.TrySerialize();
        memoryVector.insert(memoryVector.end(), memoryTagVector.begin(), memoryTagVector.end()); //MEMORY_TAG
        auto arrayValuesVector = std::vector<uint8_t>();
        for (auto arrayValue : m_values) //ARRAY_BINARY_VALUE/S
        {
            auto memoryChunk = arrayValue->TrySerialize();
            arrayValuesVector.insert(arrayValuesVector.end(), memoryChunk.begin(), memoryChunk.end());
        }
        memoryVector.push_back(memoryVector.size() + arrayValuesVector.size() + 1); //LENGTH  IN BYTES
        memoryVector.insert(memoryVector.end(), arrayValuesVector.begin(), arrayValuesVector.end());
        return memoryVector;
    }

    std::vector<std::shared_ptr<INetworkType>> NetArray::Deserialize(const uint8_t *argsMemory)
    {
        std::vector<std::shared_ptr<INetworkType>> objectStructure;
        auto memoryTag = m_instance_name.Deserialize(argsMemory).at(0);
        auto memoryTagOffset = memoryTag->GetSize();
        auto arrayLength = argsMemory[memoryTagOffset + MEM_OFFSET_1];
        auto startOffset = memoryTagOffset + MEM_OFFSET_2;
        auto valuesMemoryVector = std::vector<uint8_t>(argsMemory + startOffset, (argsMemory + arrayLength));
        auto decodedArrayVal = binaryParser.Decode(valuesMemoryVector);
        objectStructure.push_back(std::make_shared<MAP::NetArray>(decodedArrayVal, memoryTag->GetName()));
        return objectStructure;
    }

    NetworkType NetArray::GetType()
    {
        return NetworkType::ARRAY;
    }

    const char *NetArray::GetName()
    {
        return m_instance_name.GetName();
    }

    uint32_t NetArray::GetSize()
    {
        std::size_t partialSize = 0;
        for (auto arrayVal : m_values)
            partialSize += arrayVal->GetSize();
        return m_instance_name.GetSize() + partialSize;
    }

    std::vector<std::shared_ptr<MAP::INetworkType>> NetArray::GetValues()
    {
        return m_values;
    }

    std::shared_ptr<MAP::INetworkType> NetArray::At(std::size_t pos)
    {
        return m_values.at(pos);
    }

}