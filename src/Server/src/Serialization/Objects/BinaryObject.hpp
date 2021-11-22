#ifndef MAP_BINARY_OBJECT_H
#define MAP_BINARY_OBJECT_H
#include "../../../include/IMapObject.hpp"
#include <SerializerAPI.hpp>

#include <Types/String.hpp>
#include <Types/Integer.hpp>
#include <Types/Float.hpp>
#include <Types/Byte.hpp>

namespace MAP
{
    class BinaryObject : public IMapObject
    {
    public:
        BinaryObject() {
            
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
            return 255;
        };

        int32_t GetInt32() override
        {
            return -1;
        };

        std::string GetString() override
        {
            return std::string("null");
        };

        std::vector<std::shared_ptr<IMapObject>> GetArray(const char *fieldName) override
        {
            return std::vector<std::shared_ptr<IMapObject>>();
        }

        std::shared_ptr<IMapObject> At(std::size_t index) override
        {
            return std::shared_ptr<IMapObject>();
        }

        //Setters
        void SetString(const char *fieldName, const std::string &value) override
        {
            if (!m_is_initialized)
            {
                m_binary_object = std::make_shared<MAP::NetString>(value, fieldName);
                m_is_initialized = true;
            }
        };

        void SetByte(const char *fieldName, const uint8_t value) override
        {
            if (!m_is_initialized)
            {
                m_binary_object = std::make_shared<MAP::NetByte>(value, fieldName);
                m_is_initialized = true;
            }
        };

        void SetInt32(const char *fieldName, const int32_t value) override
        {
            if (!m_is_initialized)
            {
                m_binary_object = std::make_shared<MAP::NetInt>(value, fieldName);
                m_is_initialized = true;
            }
        };

        void SetArray(const char *fieldName, std::vector<std::shared_ptr<IMapObject>> value) override
        {
            //NOT IMPLEMENTED WTFF
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