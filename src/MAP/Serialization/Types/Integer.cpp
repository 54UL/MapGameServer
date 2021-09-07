#include "Integer.hpp"

namespace MAP
{
    NetInt::NetInt() : m_value(0), m_instance_name(std::string("INT-SYSTEM"))
    {
    }

    NetInt::NetInt(int value, std::string name) : m_value(value), m_instance_name(name)
    {
    }
    NetInt::~NetInt()
    {
    }

    std::vector<uint8_t> NetInt::Serialize()
    {
        std::vector<uint8_t> memoryVector;
        memoryVector.push_back((uint8_t)GetType());
        auto memoryTagVector = m_instance_name.Serialize();
        memoryVector.insert(memoryVector.end(), memoryTagVector.begin(), memoryTagVector.end());
        auto intSize = sizeof(m_value);
        auto intBytePtr = reinterpret_cast<uint8_t *>(&m_value);
        for (uint8_t findex = 0; findex < intSize; findex++)
        {
            memoryVector.push_back(intBytePtr[findex]);
        }
        return memoryVector;
    }

    std::vector<std::shared_ptr<INetworkType>> NetInt::Deserialize(const uint8_t *argsMemory)
    {
        std::vector<std::shared_ptr<INetworkType>> objectStructure;
        auto memoryTag = m_instance_name.Deserialize(argsMemory).at(0);
        auto intStartPos = memoryTag->GetSize() + MEM_OFFSET_1;

        const uint8_t *intPtr = argsMemory + intStartPos;
        m_value = *reinterpret_cast<const int *>(intPtr);

        objectStructure.push_back(std::make_shared<MAP::NetInt>(m_value, memoryTag->GetName()));
        return objectStructure;
    }

    NetworkType NetInt::GetType()
    {
        return NetworkType::INT;
    }

    const char *NetInt::GetName()
    {
        return m_instance_name.GetName();
    }

    uint32_t NetInt::GetSize()
    {
        return m_instance_name.GetSize() + sizeof(m_value)+1;//+1 for length byte
    }

    int NetInt::GetValue()
    {
        return m_value;
    }
}