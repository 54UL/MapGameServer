#ifndef SERIALIZER_TEST
#define SERIALIZER_TEST

#include <BinaryUtils.hpp>
#include <gtest/gtest.h>

#include <memory>
#include <vector>
using namespace MAP;

TEST(MapSerializer, ByteTypeTest)
{
    NetworkObject sequence = {
        std::make_shared<MAP::NetCommand>(1, 102),
        std::make_shared<MAP::NetByte>(42, "answer"),
        std::make_shared<MAP::NetByte>(128, "halfbyte"),
        std::make_shared<MAP::NetByte>(255, "fullbyte")};

    auto binaryData = BinaryUtils::Encode(sequence);
    auto deserialized = BinaryUtils::DecodeAsMap(binaryData.data(), binaryData.size());

    auto half = std::dynamic_pointer_cast<MAP::NetByte>(deserialized["halfbyte"]);
    auto full = std::dynamic_pointer_cast<MAP::NetByte>(deserialized["fullbyte"]);
    auto answer = std::dynamic_pointer_cast<MAP::NetByte>(deserialized["answer"]);

    auto test1 = half->GetValue();
    auto test2 = full->GetValue();
    auto test3 = answer->GetValue();

    EXPECT_EQ(test1 == 128 && test2 == 255 && test3 == 42, true);
}

TEST(MapSerializer, DynamicArrayTypeTest)
{
    NetworkObject sequence = {
        std::make_shared<MAP::NetByte>(42, "byteVal"),
        std::make_shared<MAP::NetFloat>(0.33f, "decimal"),
        std::make_shared<MAP::NetString>("XXXXXXXXXXXXXXXXXXXXXXXXXX", "someString")};

    auto arrayObj = std::make_shared<MAP::NetArray>(sequence, "arrayTest");
    auto serializedArrayVector = arrayObj->Serialize();
    auto objMap = BinaryUtils::DecodeAsMap(serializedArrayVector.data(), serializedArrayVector.size());
    auto arr = std::dynamic_pointer_cast<MAP::NetArray>(objMap["arrayTest"])->GetValues();

    auto byte = BinaryUtils::Get<MAP::NetByte>(arr, "byteVal")->GetValue();
    auto floatVal = BinaryUtils::Get<MAP::NetFloat>(arr, "decimal")->GetValue();
    auto stringVal = BinaryUtils::Get<MAP::NetString>(arr, "someString")->GetValue();

    EXPECT_EQ(byte, 42);
    EXPECT_EQ(floatVal, 0.33f);
    EXPECT_STREQ(stringVal.c_str(), "XXXXXXXXXXXXXXXXXXXXXXXXXX");
}

TEST(MapSerializer, StringTypeTest)
{
    NetworkObject sequence = {
        std::make_shared<MAP::NetString>("Hello world", "hello"),
        std::make_shared<MAP::NetString>("This is a large text, can contain 255 characters", "longText")};
    auto serializedStrings = BinaryUtils::Encode(sequence);
    auto deserialized = BinaryUtils::DecodeAsMap(serializedStrings.data(), serializedStrings.size());
    auto hello = std::dynamic_pointer_cast<MAP::NetString>(deserialized["hello"]);
    auto longText = std::dynamic_pointer_cast<MAP::NetString>(deserialized["longText"]);
    auto helloString = hello->GetValue();
    auto largeString = longText->GetValue();
    EXPECT_TRUE(helloString.compare("Hello world") == 0);
    EXPECT_TRUE(largeString.compare("This is a large text, can contain 255 characters") == 0);
}

TEST(MapSerializer, FloatTypeTest)
{
    NetworkObject sequence = {
        std::make_shared<MAP::NetFloat>(0.006f, "tiny6"),
        std::make_shared<MAP::NetFloat>(42.0f, "abc"),
        std::make_shared<MAP::NetFloat>(128.0f, "half"),
        std::make_shared<MAP::NetFloat>(255.0f, "full")};

    auto serializedFloats = BinaryUtils::Encode(sequence);
    auto deserialized = BinaryUtils::DecodeAsMap(serializedFloats.data(), serializedFloats.size());

    auto tinyVal = BinaryUtils::Get<MAP::NetFloat>(deserialized, "tiny6");
    auto halfVal = BinaryUtils::Get<MAP::NetFloat>(deserialized, "half");

    float val = tinyVal->GetValue();
    float val2 = halfVal->GetValue();

    EXPECT_EQ(val, 0.006f);
    EXPECT_EQ(val2, 128.0f);
}

TEST(MapSerializer, Int32TypeTest)
{
    NetworkObject sequence = {
        std::make_shared<MAP::NetInt>(777, "luck"),
        std::make_shared<MAP::NetInt>(666, "hell"),
        std::make_shared<MAP::NetInt>(12345678, "large")};

    auto serializedInts = BinaryUtils::Encode(sequence);
    auto deserialized = BinaryUtils::DecodeAsMap(serializedInts.data(), serializedInts.size());

    auto luck = BinaryUtils::Get<MAP::NetInt>(deserialized, "luck");
    auto hell = BinaryUtils::Get<MAP::NetInt>(deserialized, "hell");
    auto large = BinaryUtils::Get<MAP::NetInt>(deserialized, "large");

    int val = luck->GetValue();
    int val2 = hell->GetValue();
    int val3 = large->GetValue();

    EXPECT_EQ(val, 777);
    EXPECT_EQ(val2, 666);
    EXPECT_EQ(val3, 12345678);
}

TEST(MapSerializer, ComplexArrayObjectTest)
{
    NetworkObject poolData{
        std::make_shared<MAP::NetInt>(666, "poolId"),
        std::make_shared<MAP::NetString>("Example name", "poolName")};

    NetworkObject
        commandPayload{
            std::make_shared<MAP::NetArray>(poolData, "AAAAA"),
            std::make_shared<MAP::NetArray>(poolData, "BBBB")};

    auto serializedObject = BinaryUtils::Encode(commandPayload);
    auto deserializedObjectMap = BinaryUtils::DecodeAsMap(serializedObject.data(), serializedObject.size());
    //Get pool obj3
    auto poolDataArr = BinaryUtils::Get<MAP::NetArray>(deserializedObjectMap, "BBBB")->GetValues();
    auto poolId = BinaryUtils::Get<MAP::NetInt>(poolDataArr, "poolId")->GetValue();
    auto poolName = BinaryUtils::Get<MAP::NetString>(poolDataArr, "poolName")->GetValue();
    EXPECT_EQ(poolId, 666);
    EXPECT_TRUE(poolName.compare("Example name") == 0);
}

TEST(MapSerializer, StaticArrayTest)
{
    //Float testing (emulates a vector 3)
    NetworkObject sequence = {
        std::make_shared<MAP::NetFloat>(0.42f, ""),
        std::make_shared<MAP::NetFloat>(0.33f, ""),
        std::make_shared<MAP::NetFloat>(0.42069f, "")};

    auto arrayObj = std::make_shared<MAP::NetStaticArray>(sequence, NetworkType::FLOAT, "sarrayTest");
    auto serializedArrayVector = arrayObj->Serialize();
    auto objMap = BinaryUtils::DecodeAsMap(serializedArrayVector.data(), serializedArrayVector.size());
    auto arr = std::dynamic_pointer_cast<MAP::NetStaticArray>(objMap["sarrayTest"])->GetValues();

    auto first = std::dynamic_pointer_cast<MAP::NetFloat>(arr.at(0))->GetValue();
    auto second = std::dynamic_pointer_cast<MAP::NetFloat>(arr.at(1))->GetValue();
    auto third = std::dynamic_pointer_cast<MAP::NetFloat>(arr.at(2))->GetValue();

    EXPECT_EQ(first, 0.42f);
    EXPECT_EQ(second, 0.33f);
    EXPECT_EQ(third, 0.42069f);

    //String type test
    NetworkObject sequence2 = {
        std::make_shared<MAP::NetString>("HELLO", ""),
        std::make_shared<MAP::NetString>("world", ""),
        std::make_shared<MAP::NetString>("XxXx  XxXxXx", "")};

    auto arrayObj2 = std::make_shared<MAP::NetStaticArray>(sequence2, NetworkType::STRING, "sarrayTest2");
    auto serializedArrayVector2 = arrayObj2->Serialize();
    auto objMap2 = BinaryUtils::DecodeAsMap(serializedArrayVector2.data(), serializedArrayVector2.size());
    auto arr2 = std::dynamic_pointer_cast<MAP::NetStaticArray>(objMap2["sarrayTest2"])->GetValues();

    auto firstString = std::dynamic_pointer_cast<MAP::NetString>(arr2.at(0))->GetValue();
    auto secondString = std::dynamic_pointer_cast<MAP::NetString>(arr2.at(1))->GetValue();
    auto thirdString = std::dynamic_pointer_cast<MAP::NetString>(arr2.at(2))->GetValue();

    EXPECT_TRUE(firstString.compare("HELLO") == 0);
    EXPECT_TRUE(secondString.compare("world") == 0);
    EXPECT_TRUE(thirdString.compare("XxXx  XxXxXx") == 0);

    //Int type test
    NetworkObject sequence3 = {
        std::make_shared<MAP::NetInt>(111111, ""),
        std::make_shared<MAP::NetInt>(222222, ""),
        std::make_shared<MAP::NetInt>(333333, "")};

    auto arrayObj3 = std::make_shared<MAP::NetStaticArray>(sequence3, NetworkType::INT, "sarrayTest3");
    auto serializedArrayVector3 = arrayObj3->Serialize();
    auto objMap3 = BinaryUtils::DecodeAsMap(serializedArrayVector3.data(), serializedArrayVector3.size());
    auto arr3 = std::dynamic_pointer_cast<MAP::NetStaticArray>(objMap3["sarrayTest3"])->GetValues();

    auto firstInt = std::dynamic_pointer_cast<MAP::NetInt>(arr3.at(0))->GetValue();
    auto secondInt = std::dynamic_pointer_cast<MAP::NetInt>(arr3.at(1))->GetValue();
    auto thirdInt = std::dynamic_pointer_cast<MAP::NetInt>(arr3.at(2))->GetValue();
    EXPECT_EQ(firstInt, 111111);
    EXPECT_EQ(secondInt, 222222);
    EXPECT_EQ(thirdInt, 333333);

    //Byte type test
    NetworkObject sequence4 = {
        std::make_shared<MAP::NetByte>(1, ""),
        std::make_shared<MAP::NetByte>(128, ""),
        std::make_shared<MAP::NetByte>(255, "")};

    auto arrayObj4 = std::make_shared<MAP::NetStaticArray>(sequence4, NetworkType::BYTE, "sarrayTest4");
    auto serializedArrayVector4 = arrayObj4->Serialize();
    auto objMap4 = BinaryUtils::DecodeAsMap(serializedArrayVector4.data(), serializedArrayVector4.size());
    auto arr4 = std::dynamic_pointer_cast<MAP::NetStaticArray>(objMap4["sarrayTest4"])->GetValues();

    auto firstByte = std::dynamic_pointer_cast<MAP::NetByte>(arr4.at(0))->GetValue();
    auto secondByte = std::dynamic_pointer_cast<MAP::NetByte>(arr4.at(1))->GetValue();
    auto thirdByte = std::dynamic_pointer_cast<MAP::NetByte>(arr4.at(2))->GetValue();

    EXPECT_EQ(firstByte, 1);
    EXPECT_EQ(secondByte, 128);
    EXPECT_EQ(thirdByte, 255);
}

TEST(MapSerializer, TypeAssignmentTest)
{
    // THIS IS A ASSIGNMENT TEST WITHOUT OPERATOR OVERLOAD IN THE INTERFACE(TODO!!!)
    std::map<std::string, std::shared_ptr<MAP::INetworkType>> testMap{
        std::make_pair("element1", std::make_shared<MAP::NetInt>(666, "PoolId")),
        std::make_pair("element2", std::make_shared<MAP::NetInt>(666, "PoolId"))};

    auto someExternalNetworkType = std::make_shared<MAP::NetInt>(33333, "PoolId");
    testMap["element1"] = someExternalNetworkType;
    int testMapVal = std::dynamic_pointer_cast<MAP::NetInt>(testMap["element1"])->GetValue();
    EXPECT_EQ(testMapVal, 33333);
}

#endif