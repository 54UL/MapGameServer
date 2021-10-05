
#ifndef BYTE_TYPE_H
#define BYTE_TYPE_H
#include "./MemoryTag.hpp"
#include <string>

namespace MAP
{
    class NetByte : public INetworkType
    {
    public:
        NetByte();
        NetByte(uint8_t value);
        NetByte(uint8_t value, std::string name);
        ~NetByte();

        std::vector<uint8_t> RawSerialization() override;
        std::vector<uint8_t> Serialize() override;
        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override;
        std::vector<std::shared_ptr<INetworkType>> RawDeserialization(std::vector<uint8_t> argsMemory) override;

        NetworkType GetType() override;
        const char *GetName() override;
        uint32_t GetRawSize() override;
        uint32_t GetSize() override;
        uint8_t GetValue();
    private:
        uint8_t m_value;
        MAP::MemoryTag m_instance_name;
    };
}

#endif