
#ifndef COMMAND_TYPE_H
#define COMMAND_TYPE_H

#include "../SerializerAPI.hpp"
#include "./Byte.hpp"
#include <vector>
#include <memory>


namespace MAP
{
    class NetCommand : public INetworkType
    {
    public:
        NetCommand() : m_command_id(0)
        {
            m_client_id = 255;
        }

        NetCommand(uint8_t commandId,uint8_t clientId) : m_command_id(commandId)
        {
            m_client_id = clientId;
        }

        ~NetCommand()
        {
        }

        std::vector<uint8_t> TrySerialize() override
        {
            std::vector<uint8_t> memoryVector(0);
            memoryVector.push_back(static_cast<uint8_t>(GetType()));
            memoryVector.push_back(m_command_id);
            memoryVector.push_back(m_client_id);
            return memoryVector;
        }

        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override
        {
            auto commandId = argsMemory[MEM_OFFSET_1];
            auto clientId = argsMemory[MEM_OFFSET_2];
            std::vector<std::shared_ptr<MAP::INetworkType>> objectStructure;
            objectStructure.push_back(std::make_shared<NetCommand>(commandId,clientId));
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