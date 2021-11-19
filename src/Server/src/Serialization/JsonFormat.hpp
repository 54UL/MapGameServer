#ifndef MAP_BINARYFORMAT_H
#define MAP_BINARYFORMAT_H
#include "../../include/IMapDataFormat.hpp"

namespace MAP
{
    class MapBinaryFormat : public IMapDataFormat
    {
    public:
        MapBinaryFormat(){}
        virtual ~MapBinaryFormat() {}
        
    };
} // namespace MAP

#endif