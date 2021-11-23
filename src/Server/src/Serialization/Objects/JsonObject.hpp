#ifndef MAP_JSON_OBJECT_H
#define MAP_JSON_OBJECT_H
#include "../../../include/IMapObject.hpp"

namespace MAP
{
    class JsonObject : public IMapObject
    {
    public:
        JsonObject() {}
        virtual ~JsonObject() {
            
        }
        //Getters
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
            return std::shared_ptr<IMapObject>();
        }

        // //Setters
        // void SetString(const char *fieldName, const std::string &value) override{

        // };

        // void SetByte(const char *fieldName, const uint8_t value) override{

        // };

        // void SetInt32(const char *fieldName, const int32_t value) override{

        // };
        // void SetArray(const char *fieldName, std::vector<std::shared_ptr<IMapObject>> value) override
        // {
        // }

    private:
        bool isInitialized = false;
    };
} // namespace MAP

#endif