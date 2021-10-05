
#include "Command.hpp"

namespace MAP
{
    NetCommand::NetCommand() : m_command_id(0)
    {
        m_client_id = 255;
    }

    NetCommand::NetCommand(uint8_t commandId, uint8_t clientId) : m_command_id(commandId)
    {
        m_client_id = clientId;
    }

    NetCommand::~NetCommand()
    {
    }

    std::vector<uint8_t> NetCommand::RawSerialization()
    {
        return std::vector<uint8_t>() = {
                   m_command_id,
                   m_client_id};
    }

    std::vector<uint8_t> NetCommand::Serialize()
    {
        std::vector<uint8_t> memoryVector(0);
        memoryVector.push_back(static_cast<uint8_t>(GetType()));
        memoryVector.push_back(m_command_id);
        memoryVector.push_back(m_client_id);
        return memoryVector;
    }

    std::vector<std::shared_ptr<INetworkType>> NetCommand::RawDeserialization(std::vector<uint8_t> argsMemory)
    {
        return std::vector<std::shared_ptr<INetworkType>>() = {std::make_shared<NetCommand>(argsMemory[0], argsMemory[1])};
    }

    std::vector<std::shared_ptr<INetworkType>> NetCommand::Deserialize(const uint8_t *argsMemory)
    {
        auto commandId = argsMemory[MEM_OFFSET_1];
        auto clientId = argsMemory[MEM_OFFSET_2];
        NetworkObject objectStructure;
        objectStructure.push_back(std::make_shared<NetCommand>(commandId, clientId));
        return objectStructure;
    }

    const char *NetCommand::GetName()
    {
        return "COMMAND";
    }

    NetworkType NetCommand::GetType()
    {
        return NetworkType::COMMAND;
    }

    uint32_t NetCommand::GetRawSize(){
        return sizeof(m_command_id) + sizeof(m_client_id);
    }

    uint32_t NetCommand::GetSize()
    {
        return GetRawSize() + 1; //+1 for the type byte
    }

    uint8_t NetCommand::id()
    {
        return m_command_id;
    }

    uint8_t NetCommand::clientId()
    {
        return m_client_id;
    }
}