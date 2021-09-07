#include "String.hpp"

namespace MAP
{
    NetString::NetString() : m_instance_name(std::string("STRING-SYSTEM"))
    {
    }

    NetString::NetString(const std::string &value, const std::string &name) : m_instance_name(name), m_string_value(value)
    {
    }

    NetString::~NetString()
    {
    }

    std::vector<uint8_t> NetString::Serialize()
    {
        std::vector<uint8_t> memoryVector(0);
        memoryVector.push_back((uint8_t)GetType());
        auto memoryTagVector = m_instance_name.Serialize();
        memoryVector.insert(memoryVector.end(), memoryTagVector.begin(), memoryTagVector.end());
        //INSERT STRING VALUE
        memoryVector.push_back(static_cast<uint8_t>(m_string_value.length())); //Length string
        for (auto stringIterator : m_string_value)                             //String data
        {
            memoryVector.push_back(stringIterator);
        }
        return memoryVector;
    }

    std::vector<std::shared_ptr<INetworkType>> NetString::Deserialize(const uint8_t *argsMemory)
    {
        m_string_value = "";
        std::vector<std::shared_ptr<INetworkType>> objectStructure;
        auto memoryTag = m_instance_name.Deserialize(argsMemory).at(0);
        auto memoryTagOffset = memoryTag->GetSize();
        auto stringLength = argsMemory[memoryTagOffset + MEM_OFFSET_1];
        for (uint8_t i = 0; i < stringLength; i++)
        {
            m_string_value.push_back(argsMemory[i + memoryTagOffset + MEM_OFFSET_2]);
        }
        objectStructure.push_back(std::make_shared<MAP::NetString>(m_string_value, memoryTag->GetName()));
        return objectStructure;
    }

    NetworkType NetString::GetType()
    {
        return NetworkType::STRING;
    }

    const char *NetString::GetName()
    {
        return m_instance_name.GetName();
    }

    uint32_t NetString::GetSize()
    {
        return m_instance_name.GetSize() + m_string_value.size() + 2; //+2 is for type byte and length byte
    }

    std::string NetString::GetValue()
    {
        return m_string_value;
    }
}