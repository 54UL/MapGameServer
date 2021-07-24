#ifndef SERIALIIZER_API_H
#define SERIALIIZER_API_H

#include <stdint.h>
#include <memory>
#include <vector>
#include <map>

namespace MAP
{
    enum class NetworkType : uint8_t
    {
        COMMAND = 0x01,
        STRING = 0x02,
        FLOAT = 0x03,
        ARRAY = 0x04,
        BYTE = 0x05,
        INT = 0x06,
        UINT = 0x07
    };

    class INetworkType
    {
    public:
        INetworkType() {}
        virtual ~INetworkType() {}
        // virtual bool TrySerialize(std::vector<uint8_t> memoryVector) = 0;
        virtual bool TrySerialize(Serializer *serializer) = 0;

        // virtual std::vector<std::shared_ptr<INetworkType>> Deserialize(std::vector<uint8_t> payloadData, uint16_t lastPayloadPosition) = 0;
        virtual std::vector<std::shared_ptr<INetworkType>> Deserialize(Serializer *serializer) = 0;
        virtual NetworkType GetType() = 0;
        virtual uint32_t GetSize() = 0;
    };
}

#endif