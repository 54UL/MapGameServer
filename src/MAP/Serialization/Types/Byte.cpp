#include "Byte.hpp"

namespace MAP
{
    NetByte::NetByte() : m_value(0)
    {
    }
    NetByte::NetByte(uint8_t value) : m_value(value), instance_name_("SYSTEM-BYTE")
    {
    }
    NetByte::NetByte(uint8_t value, std::string name) : m_value(value), instance_name_(name)
    {
    }
    NetByte::~NetByte()
    {
    }

    std::vector<uint8_t> NetByte::TrySerialize()
    {
        std::vector<uint8_t> memoryVector(0);
        memoryVector.push_back((uint8_t)GetType());
        auto memoryTagVector = instance_name_.TrySerialize();
        memoryVector.insert(memoryVector.end(), memoryTagVector.begin(), memoryTagVector.end());
        memoryVector.push_back(m_value);
        return memoryVector;
    }

    std::vector<std::shared_ptr<INetworkType>> NetByte::Deserialize(const uint8_t *argsMemory)
    {
        std::vector<std::shared_ptr<INetworkType>> objectStructure;
        auto memoryTag = instance_name_.Deserialize(argsMemory).at(0);
        objectStructure.push_back(std::make_shared<MAP::NetByte>(argsMemory[memoryTag->GetSize() + MEM_OFFSET_1], memoryTag->GetName()));
        return objectStructure;
    }

    NetworkType NetByte::GetType()
    {
        return NetworkType::BYTE;
    }

    const char *NetByte::GetName()
    {
        return instance_name_.GetName();
    }

    uint32_t NetByte::GetSize()
    {
        return instance_name_.GetSize() + sizeof(m_value); + 1; //
    }

    uint8_t NetByte::GetValue()
    {
        return m_value;
    }
}