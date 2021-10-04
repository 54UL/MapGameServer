#include "Byte.hpp"

namespace MAP
{
    NetByte::NetByte() : m_value(0)
    {
    }
    NetByte::NetByte(uint8_t value) : m_value(value), m_instance_name("SYSTEM-BYTE")
    {
    }
    NetByte::NetByte(uint8_t value, std::string name) : m_value(value), m_instance_name(name)
    {
    }
    NetByte::~NetByte()
    {
    }

    std::vector<uint8_t> NetByte::RawSerialization()
    {
        return std::vector<uint8_t>() = {m_value};
    }

    std::vector<uint8_t> NetByte::Serialize()
    {
        std::vector<uint8_t> memoryVector;
        memoryVector.push_back((uint8_t)GetType());
        auto memoryTagVector = m_instance_name.Serialize();
        memoryVector.insert(memoryVector.end(), memoryTagVector.begin(), memoryTagVector.end());
        memoryVector.push_back(m_value);
        return memoryVector;
    }

    std::vector<std::shared_ptr<INetworkType>> NetByte::RawDeserialization(std::vector<uint8_t> argsMemory)
    {
        return std::vector<std::shared_ptr<INetworkType>>() = {std::make_shared<MAP::NetByte>(argsMemory[0], "NULL")};
    }

    std::vector<std::shared_ptr<INetworkType>> NetByte::Deserialize(const uint8_t *argsMemory)
    {
        std::vector<std::shared_ptr<INetworkType>> objectStructure;
        auto memoryTag = m_instance_name.Deserialize(argsMemory).at(0);
        objectStructure.push_back(std::make_shared<MAP::NetByte>(argsMemory[memoryTag->GetSize() + MEM_OFFSET_1], memoryTag->GetName()));
        return objectStructure;
    }

    NetworkType NetByte::GetType()
    {
        return NetworkType::BYTE;
    }

    const char *NetByte::GetName()
    {
        return m_instance_name.GetName();
    }

    uint32_t NetByte::GetRawSize(){
        return sizeof(m_value);    
    }

    uint32_t NetByte::GetSize()
    {
        return m_instance_name.GetSize() + sizeof(m_value) + 1;//+1 for length byte
    }

    uint8_t NetByte::GetValue()
    {
        return m_value;
    }
}