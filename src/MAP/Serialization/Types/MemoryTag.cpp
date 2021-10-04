#include "MemoryTag.hpp"

namespace MAP
{
    MemoryTag::MemoryTag() : m_instance_name(std::string("MEMORY-TAG-SYSTEM"))
    {
    }

    MemoryTag::MemoryTag(const std::string &name) : m_instance_name(name)
    {
    }

    MemoryTag::~MemoryTag()
    {
    }

    std::vector<uint8_t> MemoryTag::RawSerialization()
    {
         std::vector<uint8_t> memoryVector(0);
        //NOTE: MEMORY TAG DOES NOT HAVE TO PASS HIS TYPE ID (ASSUMPTION DUE TO COMPOSITION)
        memoryVector.push_back(static_cast<uint8_t>(m_instance_name.length())); //CHANGE THIS CAST of size_t by uint8_t
        for (auto stringIterator : m_instance_name)
        {
            memoryVector.push_back(stringIterator);
        }
        return memoryVector;
    }

    std::vector<uint8_t> MemoryTag::Serialize()
    {
        return RawSerialization();
    }

    std::vector<std::shared_ptr<INetworkType>> MemoryTag::RawDeserialization(std::vector<uint8_t> argsMemory)
    {
        std::vector<std::shared_ptr<INetworkType>> objectStructure;
        uint8_t tagLength = argsMemory[MEM_OFFSET_1]; //lenght first pos
        m_instance_name = "";
        for (uint8_t i = 0; i < tagLength; i++)
        {
            m_instance_name.push_back(argsMemory[MEM_OFFSET_2 + i]);
        }
        objectStructure.push_back(std::make_shared<MAP::MemoryTag>(m_instance_name));
        return objectStructure;
    }

    std::vector<std::shared_ptr<INetworkType>> MemoryTag::Deserialize(const uint8_t *argsMemory)
    {
        std::vector<std::shared_ptr<INetworkType>> objectStructure;
        uint8_t tagLength = argsMemory[MEM_OFFSET_1]; //lenght first pos
        m_instance_name = "";
        for (uint8_t i = 0; i < tagLength; i++)
        {
            m_instance_name.push_back(argsMemory[MEM_OFFSET_2 + i]);
        }
        objectStructure.push_back(std::make_shared<MAP::MemoryTag>(m_instance_name));
        return objectStructure;
    }

    NetworkType MemoryTag::GetType()
    {
        return NetworkType::TAG;
    }

    const char *MemoryTag::GetName()
    {
        return m_instance_name.c_str();
    }

    uint32_t MemoryTag::GetRawSize(){
        return m_instance_name.size() + 1; //1 byte extra for the size byte
    }

    uint32_t MemoryTag::GetSize()
    {
        return GetRawSize();
    }
}