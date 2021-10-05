
#ifndef NET_STATIC_ARRAY_TYPE
#define NET_STATIC_ARRAY_TYPE
#include <string>
#include <vector>
#include <memory>
#include "MemoryTag.hpp"

namespace MAP
{
    class NetStaticArray : public INetworkType
    {
    public:
        NetStaticArray();
        NetStaticArray(NetworkObject sequence,NetworkType contentType, std::string name);
        virtual ~NetStaticArray();

        std::vector<uint8_t> RawSerialization() override;
        std::vector<uint8_t> Serialize() override;
        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override;
        std::vector<std::shared_ptr<INetworkType>> RawDeserialization(std::vector<uint8_t> argsMemory) override;
        NetworkType GetType() override;
        const char *GetName() override;
        uint32_t GetRawSize() override;
        uint32_t GetSize() override;
        NetworkObject GetValues();
        std::shared_ptr<MAP::INetworkType> At(std::size_t pos);
    private:
        NetworkObject StaticDeserialization(std::vector<uint8_t> argsMemory);
    private:
        NetworkObject m_values;
        NetworkType  m_array_content_type;
        MAP::MemoryTag m_instance_name;
    };
}

#endif