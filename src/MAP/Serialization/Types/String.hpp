
#ifndef STRING_NET_TYPE
#define STRING_NET_TYPE

#include "../Types/MemoryTag.hpp"
#include "../SerializerAPI.hpp"
#include <string>

namespace MAP
{
    //Internal serializer class, don't use outside
    class NetString : public INetworkType
    {
    public:
        NetString();

        NetString(const std::string &value,const std::string &name);

        ~NetString();

        std::vector<uint8_t> Serialize() override;
        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override;
        NetworkType GetType() override;
        const char *GetName() override;
        uint32_t GetSize() override;
        std::string NetString::GetValue();
    
    private:
        MAP::MemoryTag m_instance_name;
        std::string m_string_value;
    };
}

#endif