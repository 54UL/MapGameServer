
#ifndef NET_ARRAY_TYPE
#define NET_ARRAY_TYPE
#include <string>
#include <vector>
#include <memory>

#include "MemoryTag.hpp"
namespace MAP
{
    class NetArray : public INetworkType
    {
    public:
        NetArray();
        NetArray(std::vector<std::shared_ptr<MAP::INetworkType>> sequence, std::string name);
        virtual ~NetArray();

        std::vector<uint8_t> Serialize() override;

        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override;

        NetworkType GetType() override;

        const char *GetName() override;

        uint32_t GetSize() override;
        std::vector<std::shared_ptr<MAP::INetworkType>> GetValues();
        std::shared_ptr<MAP::INetworkType> At(std::size_t pos);
    private:
        std::vector<std::shared_ptr<MAP::INetworkType>> m_values;
        MAP::MemoryTag m_instance_name;
    };
}

#endif