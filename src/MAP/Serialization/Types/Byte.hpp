
#ifndef BYTE_TYPE_H
#define BYTE_TYPE_H
#include "./Byte.hpp"
#include "./MemoryTag.hpp"
#include "../BinaryObject.hpp"

#include <string>

namespace MAP
{
    class NetByte : public INetworkType
    {
    public:
        NetByte() : m_value(0)
        {
        }
        NetByte(uint8_t value) : m_value(value), instance_name_("NONE")
        {
        }
        NetByte(uint8_t value, std::string name) : m_value(value), instance_name_(name)
        {
        }
        ~NetByte()
        {
        }

        std::vector<uint8_t> TrySerialize() override
        {
            std::vector<uint8_t> memoryVector(0);
            memoryVector.push_back((uint8_t)GetType());
            auto memoryTagVector = instance_name_.TrySerialize();
            memoryVector.insert(memoryVector.end(), memoryTagVector.begin(), memoryTagVector.end());
            memoryVector.push_back(m_value);
            return memoryVector;
        }

        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override
        {
            std::vector<std::shared_ptr<INetworkType>> objectStructure;
            auto memoryTag = instance_name_.Deserialize(argsMemory).at(0);            
            objectStructure.push_back(std::make_shared<MAP::NetByte>(argsMemory[memoryTag->GetSize()+MEM_OFFSET_1],memoryTag->GetName()));
            return objectStructure;
        }

        NetworkType GetType() override
        {
            return NetworkType::BYTE;
        }

        const char *GetName() override
        {
            return instance_name_.GetName();
        }

        uint32_t GetSize() override
        {
            return instance_name_.GetSize() + sizeof(m_value);  //+1  is the type size offset
        }

        uint8_t GetValue(){
            return m_value;
        }

    private:
        uint8_t m_value;
        MAP::MemoryTag instance_name_;
    };
}

#endif