#ifndef MAP_JSON_FORMAT_H
#define MAP_JSON_FORMAT_H
#include "../../include/IMapDataFormat.hpp"

namespace MAP
{
    class MapJsonFormat : public IMapDataFormat
    {
    public:
        MapJsonFormat() {}
        virtual ~MapJsonFormat() {}

        //Impl
        std::vector<uint8_t> Encode(std::vector<std::shared_ptr<IMapObject>> sequence) override
        {
            return std::vector<uint8_t>();
        }

        std::map<std::string, std::shared_ptr<IMapObject>> Decode(uint8_t *bytes, std::size_t length) override
        {
            return std::map<std::string, std::shared_ptr<IMapObject>>();
        }

        std::shared_ptr<IMapObject> CreateObject(uint8_t value, const char *name) override
        {
            return std::shared_ptr<IMapObject>();
        }

        std::shared_ptr<IMapObject> CreateObject(int32_t value, const char *name) override
        {
            return std::shared_ptr<IMapObject>();
        }

        std::shared_ptr<IMapObject> CreateObject(float value, const char *name) override
        {
            return std::shared_ptr<IMapObject>();
        }

        std::shared_ptr<IMapObject> CreateObject(const std::string &value, const char *name) override
        {
            return std::shared_ptr<IMapObject>();
        }

        std::shared_ptr<IMapObject> CreateObject(std::vector<std::shared_ptr<IMapObject>> value, const char *name) override
        {
            return std::shared_ptr<IMapObject>();
        }

        std::shared_ptr<IMapObject> CreateCommand(uint8_t id, uint8_t clientId) override
        {
            return std::shared_ptr<IMapObject>();
        } // id and client are 1 byte due to network perf
    };
} // namespace MAP

#endif