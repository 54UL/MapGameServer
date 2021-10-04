#include "Float.hpp"

namespace MAP
{
    NetFloat::NetFloat() : m_instance_name("SYSTEM-FLOAT")
    {
    }

    NetFloat::NetFloat(float value, std::string name) : m_instance_name(name)
    {
        m_float.value = value;
    }

    NetFloat::~NetFloat()
    {
    }

    std::vector<uint8_t> NetFloat::RawSerialization()
    {
        std::vector<uint8_t> memoryVector;
        auto floatSize = sizeof(m_float.value);
        auto floatBytePtr = reinterpret_cast<uint8_t *>(&m_float);
        for (uint8_t findex = 0; findex < floatSize; findex++)
        {
            memoryVector.push_back(floatBytePtr[findex]);
        }
        return memoryVector;
    }
    
    std::vector<uint8_t> NetFloat::Serialize()
    {
        std::vector<uint8_t> memoryVector;
        memoryVector.push_back((uint8_t)GetType());
        auto memoryTagVector = m_instance_name.Serialize();
        memoryVector.insert(memoryVector.end(), memoryTagVector.begin(), memoryTagVector.end());
        //Float value serialization
        auto binFloat = RawSerialization();
        memoryVector.insert(memoryVector.end(),binFloat.begin(),binFloat.end());
        return memoryVector;
    }

    std::vector<std::shared_ptr<INetworkType>> NetFloat::RawDeserialization(std::vector<uint8_t> argsMemory)
    {
        m_float.value = *reinterpret_cast<const float *>(argsMemory.data());
        return std::vector<std::shared_ptr<INetworkType>>() = {
            std::make_shared<MAP::NetFloat>(m_float.value, "NULL")
        };
    }

    std::vector<std::shared_ptr<INetworkType>> NetFloat::Deserialize(const uint8_t *argsMemory)
    {
        std::vector<std::shared_ptr<INetworkType>> objectStructure;
        auto memoryTag = m_instance_name.Deserialize(argsMemory).at(0);
        auto floatValueStartPos = memoryTag->GetSize() + MEM_OFFSET_1;
        m_float.value = *reinterpret_cast<const float *>(argsMemory + floatValueStartPos);
        objectStructure.push_back(std::make_shared<MAP::NetFloat>(m_float.value, memoryTag->GetName()));
        return objectStructure;
    }

    NetworkType NetFloat::GetType()
    {
        return NetworkType::FLOAT;
    }

    const char *NetFloat::GetName()
    {
        return m_instance_name.GetName();
    }

    uint32_t NetFloat::GetRawSize() {
        return sizeof(m_float.value);
    }

    uint32_t NetFloat::GetSize()
    {
        return m_instance_name.GetSize() + GetRawSize() + 1; //+1 for length byte
    }

    float NetFloat::GetValue()
    {
        return m_float.value;
    }
}