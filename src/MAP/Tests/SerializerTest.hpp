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

        inline bool DynamicTypeArrayTest()
        {
            std::vector<std::shared_ptr<MAP::INetworkType>> sequence = {
                std::make_shared<MAP::NetByte>(42, "byteVal"),
                std::make_shared<MAP::NetFloat>(0.33f, "decimal"),
                std::make_shared<MAP::NetString>("testing string lol", "someString")};

            auto arrayObj = std::make_shared<MAP::NetArray>(sequence, "arrayTest");
            auto serializedArrayVector = arrayObj->Serialize();
            auto objMap = testObj.DecodeAsMap(serializedArrayVector.data(), serializedArrayVector.size());
            auto arr = std::dynamic_pointer_cast<MAP::NetArray>(objMap["arrayTest"])->GetValues();

            auto byte = BinaryObject::Get<MAP::NetByte>(arr,"byteVal")->GetValue();
            auto floatVal = BinaryObject::Get<MAP::NetFloat>(arr,"decimal")->GetValue();
            auto stringVal = BinaryObject::Get<MAP::NetString>(arr,"someString")->GetValue();

            return byte == 42;
        }

        inline bool StringTypeTest()
        {
            std::vector<std::shared_ptr<MAP::INetworkType>> sequence = {
                std::make_shared<MAP::NetString>("Hello world", "hello"),
                std::make_shared<MAP::NetString>("This is a large text, can contain 255 characters", "longText")};
            auto serializedStrings = binaryParser.Encode(sequence);
            auto deserialized = testObj.DecodeAsMap(serializedStrings.data(), serializedStrings.size());
            auto hello = std::dynamic_pointer_cast<MAP::NetString>(deserialized["hello"]);
            auto longText = std::dynamic_pointer_cast<MAP::NetString>(deserialized["longText"]);
            return hello->GetValue().compare("Hello world") == 0;
        }

        inline bool FloatTypeTest()
        {
            std::vector<std::shared_ptr<MAP::INetworkType>> sequence = {
                std::make_shared<MAP::NetFloat>(0.006f, "tiny6"),
                std::make_shared<MAP::NetFloat>(42.0f, "abc"),
                std::make_shared<MAP::NetFloat>(128.0f, "half"),
                std::make_shared<MAP::NetFloat>(255.0f, "full")};

            auto serializedFloats = binaryParser.Encode(sequence);
            auto deserialized = testObj.DecodeAsMap(serializedFloats.data(), serializedFloats.size());

            auto tinyVal = BinaryObject::Get<MAP::NetFloat>(deserialized, "tiny6");
            auto halfVal = BinaryObject::Get<MAP::NetFloat>(deserialized, "half");

            float val = tinyVal->GetValue();
            float val2 = halfVal->GetValue();
            return val == 0.006f;
        }

        inline bool IntTypeTest()
        {
            std::vector<std::shared_ptr<MAP::INetworkType>> sequence = {
                std::make_shared<MAP::NetInt>(777, "luck"),
                std::make_shared<MAP::NetInt>(666, "hell"),
                std::make_shared<MAP::NetInt>(12345678, "large")};

            auto serializedInts = binaryParser.Encode(sequence);
            auto deserialized = testObj.DecodeAsMap(serializedInts.data(), serializedInts.size());

            auto luck = BinaryObject::Get<MAP::NetInt>(deserialized, "luck");
            auto hell = BinaryObject::Get<MAP::NetInt>(deserialized, "hell");
            auto large = BinaryObject::Get<MAP::NetInt>(deserialized, "large");

            int val = luck->GetValue();
            int val2 = hell->GetValue();
            int val3 = large->GetValue();

            return val == 777 && val2 == 666 && val3 == 12345678;
        }

        bool Check() override
        {
            bool allRight = false;
            allRight = ByteTypeTest();
            allRight = DynamicTypeArrayTest();
            allRight = StringTypeTest();
            allRight = FloatTypeTest();
            allRight = IntTypeTest();
            return allRight;
        }

    private:
        BinaryObject testObj;
    };

}

#endif