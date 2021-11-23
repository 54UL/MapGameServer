#ifndef MAP_API
#define MAP_API

#include <map>
#include <vector>
#include <array>
#include <memory>
#include <queue>
#include <string>

#define MAX_DATA_PAYLOAD 512
#define MAX_CLIENTS 16
#define MAX_ARRAY_SIZE 1024

namespace MAP
{
    //collections and utils alias
    template <class T>
    using Vector = std::vector<T>;
    template <class T, class T1>
    using Map = std::map<T, T1>;
    template <class T, int size>
    using Array = std::array<T, MAX_ARRAY_SIZE>;
    template <class T>
    using Queue = std::queue<T>;
    using String = std::string;
    template <class T>
    using SharedPtr = std::shared_ptr<T>;

    //API
    enum class ServerCommandType : uint8_t
    {
        SUBSCRIBE = 0x01,
        UNSUBSCRIBE = 0x02,
        START_POOL = 0x03,
        END_POOL = 04,
        UPSERT = 0x05,
        REMOVE = 0x06,
        SPAWN = 0x07,
        GET_ACTIVE_POOLS = 0x08
    };
    
    enum class EncodingMethod : uint8_t
    {
        NOT_DEFINED = 0X00,
        MAP_BINARY = 0X01,
        NLOHMANN_JSON = 0x02,
        GOOGLE_PROTOBUFF = 0X03
    };

    enum class TransportMethod : uint8_t
    {
        NOT_DEFINED = 0X00,
        ASIO_SOCKETS = 0X01,
        STEAM_NETWORKING_SOCKETS = 0x02
    };

} // namespace MAP

#endif