
#ifndef BYTE_TYPE_H
#define BYTE_TYPE_H

#include "../BinaryObject.hpp"
#include "./MemoryTag.hpp"

#include <string>

namespace MAP
{
    class Byte : public INetworkType
    {
    public:
        Byte() : m_value(0)
        {
        }
        Byte(uint8_t value) : m_value(value), instance_name_("NONE")
        {
        }
        Byte(uint8_t value, std::string name) : m_value(value), instance_name_(name)
        {
        }
        ~Byte()
        {
        }

        std::vector<uint8_t> TrySerialize() override
        {
            std::vector<uint8_t> memoryVector;
            memoryVector.push_back((uint8_t)GetType());
            auto memoryTagVector = instance_name_.TrySerialize();
            memoryVector.insert(memoryVector.end(), memoryTagVector.begin(), memoryVector.end());
            memoryVector.push_back(m_value);
            return memoryVector;
        }

        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override
        {
            std::vector<std::shared_ptr<INetworkType>> objectStructure;
            auto memoryTag = instance_name_.Deserialize(argsMemory);
            uint8_t valueLocation = 0;
            for (auto type : memoryTag)
            {
                valueLocation += type->GetSize();
            }
            objectStructure.insert(objectStructure.end(), memoryTag.begin(), memoryTag.end());
            objectStructure.push_back(std::make_shared<Byte>(argsMemory[valueLocation]));
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
            return instance_name_.GetSize() + sizeof(m_value);
        }

    private:
        uint8_t m_value;
        MAP::MemoryTag instance_name_;
    };
}

#endif