#ifndef SERIALIZER_TEST
#define SERIALIZER_TEST
#include <memory>
#include <vector>
#include "../Serialization/BinaryObject.hpp"
#include "../UnitTesting.hpp"
#include "../MapServer.hpp"
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

        inline bool EncodingTest()
        {
            std::size_t index = 0;
            BinaryObject testObj;
            std::vector<std::shared_ptr<MAP::INetworkType>> sequence = {
                std::make_shared<MAP::CommandType>(static_cast<uint8_t>(MAP::ServerCommandType::SUBSCRIBE), 102),
                std::make_shared<MAP::Byte>(42, "answer"),
                std::make_shared<MAP::Byte>(128, "halfbyte"),
                std::make_shared<MAP::Byte>(255, "fullbyte")
                };

            auto binaryData = testObj.Encode(sequence);
            auto deserialized = testObj.DecodeAsMap(binaryData.data(), binaryData.size());

            auto half = std::dynamic_pointer_cast<MAP::Byte>(deserialized["halfbyte"]);
            auto full = std::dynamic_pointer_cast<MAP::Byte>(deserialized["fullbyte"]);
            auto answer = std::dynamic_pointer_cast<MAP::Byte>(deserialized["answer"]);

            auto test1 = half->GetValue();
            auto test2 = full->GetValue();
            auto test3 = answer->GetValue();

            return (test1 == 128 && test2 == 255 && test3 == 42);
        }

        inline bool DecodingTest()
        {
            return true;
        }

        bool Check() override
        {
            bool allRight = false;
            allRight = EncodingTest();
            return allRight;
        }
    };

}

#endif