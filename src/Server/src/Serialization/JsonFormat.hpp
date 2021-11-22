#ifndef MAP_JSONFORMAT_H
#define MAP_JSONFORMAT_H
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

        std::vector<std::shared_ptr<IMapObject>> Decode(uint8_t *bytes, std::size_t length) override
        {
            return std::vector<std::shared_ptr<IMapObject>>();
        }
    };
} // namespace MAP

#endif