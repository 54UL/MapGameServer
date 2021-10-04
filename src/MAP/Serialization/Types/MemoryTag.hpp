
#ifndef MEMORY_TAG_H
#define MEMORY_TAG_H

#include "../SerializerAPI.hpp"
#include <string>

namespace MAP
{
    //Internal serializer class, don't use outside
    class MemoryTag : public INetworkType
    {
    public:
        MemoryTag();

        MemoryTag(const std::string &name);

        ~MemoryTag();
        
        std::vector<uint8_t> RawSerialization() override;
        std::vector<uint8_t> Serialize() override;
        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override;
        std::vector<std::shared_ptr<INetworkType>> RawDeserialization(std::vector<uint8_t> argsMemory) override;
        NetworkType GetType() override;
        const char *GetName() override;
        uint32_t GetRawSize() override;
        uint32_t GetSize() override;

    private:
        std::string m_instance_name;
    };
}

#endif