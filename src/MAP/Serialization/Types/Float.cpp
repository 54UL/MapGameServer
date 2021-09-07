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

    std::vector<uint8_t> NetFloat::TrySerialize()
    {
        std::vector<uint8_t> memoryVector;
        memoryVector.push_back((uint8_t)GetType());
        auto memoryTagVector = m_instance_name.TrySerialize();
        memoryVector.insert(memoryVector.end(), memoryTagVector.begin(), memoryTagVector.end());
        //Float value serialization
        auto floatSize = sizeof(m_float.value);
        auto floatBytePtr = reinterpret_cast<uint8_t *>(&m_float);
        for (uint8_t findex = 0; findex < floatSize; findex++)
        {
            memoryVector.push_back(floatBytePtr[findex]);
        }
        return memoryVector;
    }

    std::vector<std::shared_ptr<INetworkType>> NetFloat::Deserialize(const uint8_t *argsMemory)
    {
        std::vector<std::shared_ptr<INetworkType>> objectStructure;
        auto memoryTag = m_instance_name.Deserialize(argsMemory).at(0);
        auto floatValueStartPos = memoryTag->GetSize() + MEM_OFFSET_1;
        
        const uint8_t *floatPtr = argsMemory + floatValueStartPos;
        m_float.value = *reinterpret_cast<const float *>(floatPtr);
        
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

    uint32_t NetFloat::GetSize()
    {
        return m_instance_name.GetSize() + sizeof(m_float.value) + 1; //+1 for length byte
    }

    float NetFloat::GetValue()
    {
        return m_float.value;
    }
}