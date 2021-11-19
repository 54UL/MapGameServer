#ifndef IMAP_OBJECT_H
#define IMAP_OBJECT_H

#include <string>
#include <ctype.h>
#include <vector>
#include <memory>

namespace MAP
{
    class IMapObject
    {
    public:
        IMapObject() {}
        virtual ~IMapObject() {}

        virtual uint8_t GetByte(const char *fieldName)
        {
            return 255;
        };

        virtual int32_t GetInt32(const char *fieldName)
        {
            return -1;
        };

        virtual std::string GetString(const char *fieldName)
        {
            return std::string("null");
        };

        virtual std::vector<std::shared_ptr<IMapObject>> GetArray(const char *fieldName)
        {
            return std::vector<std::shared_ptr<IMapObject>>();
        }

        virtual std::shared_ptr<IMapObject> At(std::size_t index)
        {
            return std::shared_ptr<IMapObject>();
        }
        
        virtual void SetString(const char *fieldName, const std::string &value){};
        virtual void SetByte(const char *fieldName, const uint8_t value){};
        virtual void SetInt32(const char *fieldName, const int32_t value){};
        virtual void SetArray(const char *fieldName, std::vector<std::shared_ptr<IMapObject>> value) {}
    };
} // namespace MAP

#endif