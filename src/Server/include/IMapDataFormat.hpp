#ifndef IMAP_COMMAND_H
#define IMAP_COMMAND_H
#include "./IMapObject.hpp"
#include <stdint.h>
#include <string>
#include <vector>
#include <memory>
#include <map>

namespace MAP
{
    class IMapDataFormat
    {
    public:
        IMapDataFormat();
        virtual ~IMapDataFormat();

        virtual void Encode(std::vector<std::shared_ptr<IMapObject>> sequence) = 0;
        virtual std::vector<std::shared_ptr<IMapObject>> Decode(std::vector<uint8_t> bytes) = 0;
        virtual std::vector<std::shared_ptr<IMapObject>> Decode(const char *bytes, std::size_t length) = 0;

        // virtual std::map<std::string, std::shared_ptr<IMapObject>> DecodeAsMap(const uint8_t *bytes, std::size_t length) = 0;
    };
}

#endif