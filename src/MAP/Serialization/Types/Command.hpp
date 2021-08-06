
#ifndef COMMAND_TYPE_H
#define COMMAND_TYPE_H

#include "../SerializerAPI.hpp"
#include "./Byte.hpp"
#include <vector>
#include <memory>


namespace MAP
{
    class CommandType : public INetworkType
    {
    public:
        CommandType() : m_command_id(0)
        {
        }

        CommandType(uint8_t commandId) : m_command_id(commandId)
        {
        }

        ~CommandType()
        {
        }

        std::vector<uint8_t> TrySerialize() override
        {
            std::vector<uint8_t> memoryVector;
            memoryVector.push_back(m_command_id);
            return memoryVector;
        }

        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override
        {
            auto commandId = argsMemory[1];
            auto clientId = argsMemory[2];
            std::vector<std::shared_ptr<MAP::INetworkType>> objectStructure;
            objectStructure.push_back(std::make_shared<CommandType>(commandId));
            objectStructure.push_back(std::make_shared<Byte>(clientId));
            return objectStructure;
        }

        const char *GetName() override
        {
            return "COMMAND";
        }

        NetworkType GetType() override
        {
            return NetworkType::COMMAND;
        }

        uint32_t GetSize() override
        {
            return sizeof(m_command_id) + sizeof(m_client_id);
        }

        uint8_t id()
        {
            return m_command_id;
        }

        uint8_t clientId()
        {
            return m_client_id;
        }

    private:
        uint8_t m_command_id;
        uint8_t m_client_id;
    };
}
#endif