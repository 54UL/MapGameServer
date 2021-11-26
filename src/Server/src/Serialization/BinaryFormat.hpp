#ifndef MAP_BINARY_FORMAT_H
#define MAP_BINARY_FORMAT_H
#include <BinaryUtils.hpp>

#include "../../include/IMapDataFormat.hpp"
#include "./Objects/BinaryObject.hpp"

namespace MAP
{
    class BinaryFormat : public IMapDataFormat
    {
    public:
        BinaryFormat() {}
        virtual ~BinaryFormat() {}

        std::vector<uint8_t> Encode(std::vector<std::shared_ptr<IMapObject>> sequence) override
        {
            std::vector<uint8_t> byteSequence;
            for (const auto &sequenceObject : sequence)
            {
                auto mapBinaryObject = std::dynamic_pointer_cast<MAP::BinaryObject>(sequenceObject);
                auto netObject = mapBinaryObject->GetUnderlyingType();
                auto currentDeserializedBytes = netObject->Serialize();
                byteSequence.insert(byteSequence.end(), currentDeserializedBytes.begin(), currentDeserializedBytes.end());
            }
            return byteSequence;
        }

        std::map<std::string, std::shared_ptr<IMapObject>> Decode(uint8_t *bytes, std::size_t length) override
        {
            std::map<std::string, std::shared_ptr<IMapObject>> mapObject;
            auto deserialized = BinaryUtils::DecodeAsMap(bytes, length);
            for (const auto &field : deserialized)
            {
                mapObject.insert(std::make_pair(field.first, std::make_shared<BinaryObject>(field.second)));
            }
            return mapObject;
        }

        std::shared_ptr<IMapObject> CreateObject(uint8_t value, const char *name) override
        {
            auto byteImpl = std::make_shared<NetByte>(value, name);
            return std::make_shared<BinaryObject>(byteImpl);
        }

        std::shared_ptr<IMapObject> CreateObject(int32_t value, const char *name) override
        {
            auto intImpl = std::make_shared<NetInt>(value, name);
            return std::make_shared<BinaryObject>(intImpl);
        }

        std::shared_ptr<IMapObject> CreateObject(float value, const char *name) override
        {
            auto floatImpl = std::make_shared<NetFloat>(value, name);
            return std::make_shared<BinaryObject>(floatImpl);
        }

        std::shared_ptr<IMapObject> CreateObject(const std::string &value, const char *name) override
        {
            auto strImpl = std::make_shared<NetString>(value, name);
            return std::make_shared<BinaryObject>(strImpl);
        }

        std::shared_ptr<IMapObject> CreateObject(std::vector<std::shared_ptr<IMapObject>> value, const char *name) override
        {
            std::vector<std::shared_ptr<INetworkType>> binaryFields;
            for (auto obj : value)
            {
                binaryFields.push_back(std::dynamic_pointer_cast<BinaryObject>(obj)->GetUnderlyingType());
            }
            auto arrImpl = std::make_shared<NetArray>(binaryFields, name);
            return std::make_shared<BinaryObject>(arrImpl);
        }
        
        //This is related to map architecture...
        std::shared_ptr<IMapObject> CreateCommand(uint8_t id, uint8_t clientId) override
        {
            auto commandImpl = std::make_shared<NetCommand>(id, clientId);
            return std::make_shared<BinaryObject>(commandImpl);
        }; // id and client are 1 byte due to network performance (changhe this!!!)
    };
} // namespace MAP

#endif