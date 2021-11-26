#ifndef IMAP_DATA_FORMAT_H
#define IMAP_DATA_FORMAT_H
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
        IMapDataFormat(){}
        virtual ~IMapDataFormat(){}

        virtual std::vector<uint8_t> Encode(std::vector<std::shared_ptr<IMapObject>> sequence) = 0;
        virtual std::map<std::string,std::shared_ptr<IMapObject>> Decode(uint8_t *bytes, std::size_t length) = 0;

        virtual std::shared_ptr<IMapObject> CreateObject(uint8_t value, const char *name) = 0;
        virtual std::shared_ptr<IMapObject> CreateObject(int32_t value, const char *name) = 0;
        virtual std::shared_ptr<IMapObject> CreateObject(float value, const char *name) = 0;
        virtual std::shared_ptr<IMapObject> CreateObject(const std::string &value, const char *name) = 0;
        virtual std::shared_ptr<IMapObject> CreateObject(std::vector<std::shared_ptr<IMapObject>> value, const char *name) = 0;
        virtual std::shared_ptr<IMapObject> CreateCommand(uint8_t id,uint8_t clientId) = 0; // id and client are 1 byte due to network performance (changhe this!!!)
    };
}

#endif