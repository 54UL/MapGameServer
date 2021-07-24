
#ifndef COMMAND_H
#define COMMAND_H

#include "../SerializerAPI.hpp"
namespace MAP
{
    class Command : public INetworkType
    {
    public:
        Command() : m_command_id(0)
        {
        }

        Command(uint8_t commandId) : m_command_id(commandId)
        {
        }

        ~Command()
        {
        }

        bool TrySerialize(Serializer *serializer) override
        {
            std::vector<uint8_t> serializedArgsStack;
            serializedArgsStack.push_back((uint8_t)GetType()); // TYPE CODE
            serializedArgsStack.push_back(m_command_id);
            serializer->AddMemoryCounter(serializedArgsStack.size()); // type offset
            serializer->PushArgs(serializedArgsStack);
            return true;
        }

        std::vector<std::shared_ptr<INetworkType>> Deserialize(Serializer *serializer) override
        {
            auto args = serializer->GetCurrentPayLoad();
            std::vector<std::shared_ptr<INetworkType>> objectStructure;
            objectStructure.push_back(std::make_shared<Command>(args.at(0)));
            return objectStructure;
        }

        NetworkType GetType() override
        {
            return NetworkType::COMMAND;
        }

        uint32_t GetSize() override
        {
            return sizeof(m_command_id);
        }

    private:
        uint8_t m_command_id;
    };
}
#endif