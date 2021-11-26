#ifndef MAP_BINARY_OBJECT_H
#define MAP_BINARY_OBJECT_H
#include "../../../include/IMapObject.hpp"
#include <SerializerAPI.hpp>

#include <Types/String.hpp>
#include <Types/Integer.hpp>
#include <Types/Float.hpp>
#include <Types/Byte.hpp>
#include <Types/Array.hpp>

namespace MAP
{
    //TODO: RENAME PREFX xxxObject from classes to Container
    class BinaryObject : public IMapObject
    {
    public:
        BinaryObject()
        {
        }

        BinaryObject(std::shared_ptr<INetworkType> instanced)
        {
            if (instanced != nullptr)
            {
                m_is_initialized = true;
                m_binary_object = instanced;
            }
        }

        virtual ~BinaryObject()
        {
        }

        //Getters
        uint8_t GetByte() override
        {
            return std::dynamic_pointer_cast<NetByte>(m_binary_object)->GetValue();
        };

        int32_t GetInt32() override
        {
            return -1;
        };

        std::string GetString() override
        {
            return std::string("null");
        };

        std::vector<std::shared_ptr<IMapObject>> GetArray() override
        {
            return std::vector<std::shared_ptr<IMapObject>>();
        }

        uint8_t GetByte(const char *name) override
        {
            return 255;
        };

        int32_t GetInt32(const char *name) override
        {
            return -1;
        };

        std::string GetString(const char *name) override
        {
            return std::string("null");
        };

        std::vector<std::shared_ptr<IMapObject>> GetArray(const char *fieldName) override
        {
            return std::vector<std::shared_ptr<IMapObject>>();
        }

        std::shared_ptr<IMapObject> At(std::size_t index) override
        {
            auto netArray = std::dynamic_pointer_cast<NetArray>(m_binary_object)->GetValues();
            return std::make_shared<BinaryObject>(netArray.at(index));
        }

        std::shared_ptr<MAP::INetworkType> GetUnderlyingType()
        {
            return m_binary_object;
        }

    private:
        bool m_is_initialized = false;
        std::shared_ptr<MAP::INetworkType> m_binary_object;
    };
} // namespace MAP

#endif