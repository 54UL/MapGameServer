#ifndef MAP_BINARYFORMAT_H
#define MAP_BINARYFORMAT_H
#include <BinaryUtils.hpp>

#include "../../include/IMapDataFormat.hpp"
#include "./Objects/BinaryObject.hpp"

namespace MAP
{
    class MapFormat : public IMapDataFormat
    {
    public:
        MapFormat() {}
        virtual ~MapFormat() {}

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

        std::vector<std::shared_ptr<IMapObject>> Decode(uint8_t *bytes, std::size_t length) override
        {
            std::vector<std::shared_ptr<IMapObject>> mapObject;
            auto deserialized = BinaryUtils::DecodeAsMap(bytes, length);
            for (const auto &field : deserialized)
            {
                mapObject.push_back(std::make_shared<BinaryObject>(field.second));
            }
            return mapObject;
        }
    };
} // namespace MAP

#endif