#ifndef MAP_BINARYFORMAT_H
#define MAP_BINARYFORMAT_H
#include "../../include/IMapDataFormat.hpp"

namespace MAP
{
    class MapFormat : public IMapDataFormat
    {
    public:
        MapFormat(){}
        virtual ~MapFormat() {}
        
    };
} // namespace MAP

#endif