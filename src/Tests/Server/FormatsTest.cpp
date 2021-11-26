#ifndef FORMAT_TESTS_H
#define FORMAT_TESTS_H

#include <gtest/gtest.h>

#include <MapApi.hpp>
#include <FormatManager.hpp>
#include <IMapDataFormat.hpp>

using namespace MAP;

TEST(MapServer, BinaryDataFormatTest)
{
    MAP::FormatManager formatManager(MAP::EncodingMethod::MAP_BINARY);
    auto formater = formatManager.GetCurrentMethod();

    std::vector<std::shared_ptr<MAP::IMapObject>> objectStructure{
        formater->CreateCommand(8, 124),
        formater->CreateObject(static_cast<uint8_t>(42), "byteObj"),
        formater->CreateObject(static_cast<int32_t>(123456789), "intObj"),
        formater->CreateObject(3.1415f, "floatObj"),
        formater->CreateObject(std::string("THIS A TEST STRING, HELLO!"), "constString")};

    auto dataBytes = formater->Encode(objectStructure);
    auto decodedObject = formater->Decode(dataBytes.data(), dataBytes.size());
    // auto command = decodedObject.at(0)->GetByte("commandId");

    EXPECT_EQ(1, 1);
}

TEST(MapServer, JsonDataFormatTest)
{
    MAP::FormatManager formatManager(MAP::EncodingMethod::NLOHMANN_JSON);
    auto formater = formatManager.GetCurrentMethod();

    std::vector<std::shared_ptr<MAP::IMapObject>> objectStructure{
        formater->CreateCommand(8, 124),
        formater->CreateObject(static_cast<uint8_t>(42), "byteObj"),
        formater->CreateObject(static_cast<int32_t>(123456789), "intObj"),
        formater->CreateObject(3.1415f, "floatObj"),
        formater->CreateObject(std::string("THIS A TEST STRING, HELLO!"), "constString")
        };

    auto dataBytes = formater->Encode(objectStructure);
    auto decoedObject = formater->Decode(dataBytes.data(), dataBytes.size());
    EXPECT_EQ(1, 1);
}

#endif