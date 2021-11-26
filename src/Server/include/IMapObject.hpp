#ifndef IMAP_OBJECT_H
#define IMAP_OBJECT_H
#include <spdlog/spdlog.h>
#include <string>
#include <ctype.h>
#include <vector>
#include <memory>

namespace MAP
{
    class IMapObject
    {
    public:
        IMapObject()
        {
        }

        virtual ~IMapObject()
        {
        }

        //Getters
        virtual uint8_t GetByte()
        {
            return 255;
        }

        virtual int32_t GetInt32()
        {
            return -1;
        }

        virtual std::string GetString()
        {
            return std::string("null");
        }

        virtual std::vector<std::shared_ptr<IMapObject>> GetArray()
        {
            return std::vector<std::shared_ptr<IMapObject>>();
        }

        virtual uint8_t GetByte(const char *name)
        {
            return 255;
        }

        virtual int32_t GetInt32(const char *name)
        {
            return -1;
        }

        virtual std::string GetString(const char *name)
        {
            return std::string("not implemented");
        }

        virtual std::vector<std::shared_ptr<IMapObject>> GetArray(const char *fieldName)
        {
            return std::vector<std::shared_ptr<IMapObject>>();
        }

        virtual std::shared_ptr<IMapObject> At(std::size_t index)
        {
            return std::shared_ptr<IMapObject>();
        }
    };
} // namespace MAP

#endif