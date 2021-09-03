#ifndef SERIALIZER_TEST
#define SERIALIZER_TEST
#include <memory>
#include <vector>
#include "../UnitTesting.hpp"
#include "../Serialization/BinaryObject.hpp"
#include "../Serialization/Types/Array.hpp"
#include <iomanip>

namespace MAP
{
    class SerializerTest : public IMapTest
    {
    public:
        SerializerTest()
        {
        }

        ~SerializerTest()
        {
        }

        void OnInit() override
        {
        }

        inline bool ByteTypeTest()
        {
            std::size_t index = 0;
            std::vector<std::shared_ptr<MAP::INetworkType>> sequence = {
                std::make_shared<MAP::NetCommand>(static_cast<uint8_t>(MAP::ServerCommandType::SUBSCRIBE), 102),
                std::make_shared<MAP::NetByte>(42, "answer"),
                std::make_shared<MAP::NetByte>(128, "halfbyte"),
                std::make_shared<MAP::NetByte>(255, "fullbyte")};

            auto binaryData = testObj.Encode(sequence);
            auto deserialized = testObj.DecodeAsMap(binaryData.data(), binaryData.size());

            auto half = std::dynamic_pointer_cast<MAP::NetByte>(deserialized["halfbyte"]);
            auto full = std::dynamic_pointer_cast<MAP::NetByte>(deserialized["fullbyte"]);
            auto answer = std::dynamic_pointer_cast<MAP::NetByte>(deserialized["answer"]);

            auto test1 = half->GetValue();
            auto test2 = full->GetValue();
            auto test3 = answer->GetValue();

            return (test1 == 128 && test2 == 255 && test3 == 42);
        }

        inline bool ArrayTest()
        {
            std::vector<std::shared_ptr<MAP::INetworkType>> sequence = {
                std::make_shared<MAP::NetByte>(42, "answer"),
                std::make_shared<MAP::NetByte>(128, "halfbyte"),
                std::make_shared<MAP::NetByte>(255, "fullbyte")};

            auto arrayObj = std::make_shared<MAP::NetArray>(sequence, "arrayTest");
            auto serializedArrayVector = arrayObj->TrySerialize();
            auto objMap = testObj.DecodeAsMap(serializedArrayVector.data(), serializedArrayVector.size());
            auto half =  std::dynamic_pointer_cast<MAP::NetArray>(objMap["halfbyte"]);
            auto byte =  std::dynamic_pointer_cast<MAP::NetByte>(half->At(1));
            return byte->GetValue() == 128;
        }

        inline bool DecodingTest()
        {
            return true;
        }

        bool Check() override
        {
            bool allRight = false;
            allRight = ByteTypeTest();
            allRight = ArrayTest();
            return allRight;
        }

    private:
        BinaryObject testObj;
    };

}

#endif