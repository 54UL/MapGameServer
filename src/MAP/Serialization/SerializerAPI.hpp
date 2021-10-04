#ifndef SERIALIZER_API_H
#define SERIALIZER_API_H

#include <stdint.h>
#include <memory>
#include <vector>
#include <map>


namespace MAP
{
    #define DLLEXPORT __declspec(dllexport)

    constexpr uint8_t MEM_OFFSET_1 = 1;
    constexpr uint8_t MEM_OFFSET_2 = 2;
    constexpr uint8_t MEM_OFFSET_3 = 3;
    constexpr uint8_t MEM_OFFSET_4 = 4;
    
    enum class NetworkType : uint8_t
    {
        TAG = 0x01,
        COMMAND = 0x02,
        STRING = 0x03,
        FLOAT = 0x04,
        ARRAY = 0x05,
        SARRAY = 0X06,
        BYTE = 0x07,
        INT = 0x08,
        UINT = 0x09,
        BLOB = 0x0A,
    };

    class INetworkType
    {
    public:
        INetworkType() {}
        virtual ~INetworkType() {}
        virtual std::vector<uint8_t> RawSerialization() = 0;
        virtual std::vector<uint8_t> Serialize() = 0;
        virtual std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) = 0;
        virtual std::vector<std::shared_ptr<INetworkType>> RawDeserialization(std::vector<uint8_t> argsMemory) = 0;
        virtual const char *GetName() = 0;
        virtual NetworkType GetType() = 0;
        virtual uint32_t GetRawSize() = 0;
        virtual uint32_t GetSize() = 0;
    };

    using NetworkObject = std::vector<std::shared_ptr<MAP::INetworkType>>; 
    using NetworkField = std::shared_ptr<MAP::INetworkType>; 

}

#endif