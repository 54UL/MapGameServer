
#ifndef BYTE_TYPE_H
#define BYTE_TYPE_H

#include "../SerializerAPI.hpp"

namespace MAP
{
    class Byte : public INetworkType
    {
    public:
        Byte() : m_value(0)
        {
        }
        Byte(uint8_t value) : m_value(value)
        {
        }
        ~Byte()
        {
        }

        bool TrySerialize(Serializer *serializer) override
        {
            std::vector<uint8_t> serializedArgsStack;
            serializedArgsStack.push_back((uint8_t)GetType()); // TYPE CODE
            serializedArgsStack.push_back(m_value);
            serializer->AddMemoryCounter(serializedArgsStack.size()); // type offset
            serializer->PushArgs(serializedArgsStack);
            return true;
        }

        std::vector<std::shared_ptr<INetworkType>> Deserialize(Serializer *serializer) override
        {
            auto args =
            std::vector<std::shared_ptr<INetworkType>> objectStructure;
            objectStructure.push_back(std::make_shared<Byte>(args.at(0)));
            return objectStructure;
        }

        NetworkType GetType() override
        {
            return NetworkType::BYTE;
        }

    private:
        uint8_t m_value;
    };
}

#endif