
#ifndef NET_INT_TYPE_H
#define NET_INT_TYPE_H
#include "./MemoryTag.hpp"
#include <string>

namespace MAP
{
    class NetInt : public INetworkType
    {
    public:
        NetInt();
        NetInt(int value, std::string name);
        ~NetInt();

        std::vector<uint8_t> Serialize() override;
        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override;
        NetworkType GetType() override;
        const char *GetName() override;
        uint32_t GetSize() override;
        int GetValue();

    private:
        int m_value;
        MAP::MemoryTag m_instance_name;
    };
}

#endif