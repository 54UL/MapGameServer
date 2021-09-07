
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
        NetCommand();
        NetCommand(uint8_t commandId, uint8_t clientId);
        ~NetCommand();

        std::vector<uint8_t> Serialize() override;
        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override;
        const char *GetName() override;
        NetworkType GetType() override;
        uint32_t GetSize() override;
        uint8_t id();
        uint8_t clientId();

    private:
        uint8_t m_command_id;
        uint8_t m_client_id;
    };
}
#endif