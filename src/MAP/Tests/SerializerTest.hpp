#ifndef SERIALIZER_TEST
#define SERIALIZER_TEST
#include <memory>
#include <vector>
#include "../Serialization/BinaryObject.hpp"
#include "../UnitTesting.hpp"
#include "../MapServer.hpp"

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
            std::vector<std::shared_ptr<MAP::INetworkType>> sequence;
            //TODO: terminar de agregar tipos
            sequence.push_back(std::make_shared<MAP::CommandType>(MAP::ServerCommandType::SUBSCRIBE));
            sequence.push_back(std::make_shared<MAP::Byte>(42));
            sequence.push_back(std::make_shared<MAP::Byte>(128));
            sequence.push_back(std::make_shared<MAP::Byte>(256));

            BinaryObject testObj;
            auto binaryData = testObj.Encode(sequence);
            while (binaryData + index != nullptr)
            {
                std::cout << "|" << std::hex << binaryData[index++] << "|";
            }

            return true;
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