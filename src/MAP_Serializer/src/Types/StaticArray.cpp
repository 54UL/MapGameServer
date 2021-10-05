#include "StaticArray.hpp"
#include "Array.hpp"
#include "../BinaryUtils.hpp"

namespace MAP
{
    NetStaticArray::NetStaticArray() : m_instance_name("SYSTEM-ARRAY")
    {
    }

    NetStaticArray::NetStaticArray(NetworkObject sequence,NetworkType contentType, std::string name) : m_instance_name(name),m_array_content_type(contentType), m_values(sequence)
    {
    }

    NetStaticArray::~NetStaticArray()
    {
    }

    std::vector<uint8_t> NetStaticArray::RawSerialization()
    {
        auto arrayValuesVector = std::vector<uint8_t>();
        for (auto arrayValue : m_values)
        {
            auto memoryChunk = arrayValue->RawSerialization();
            arrayValuesVector.insert(arrayValuesVector.end(), memoryChunk.begin(), memoryChunk.end());
        }
        return arrayValuesVector;
    }

    std::vector<uint8_t> NetStaticArray::Serialize()
    {
        std::vector<uint8_t> memoryVector;
        memoryVector.push_back((uint8_t)GetType()); //TYPE
        auto memoryTagVector = m_instance_name.Serialize();
        memoryVector.insert(memoryVector.end(), memoryTagVector.begin(), memoryTagVector.end()); //MEMORY_TAG
        memoryVector.push_back(static_cast<uint8_t>(m_array_content_type));                                            //ARRAY TYPE
        auto arrayValuesVector = RawSerialization();
        memoryVector.push_back(memoryVector.size() + arrayValuesVector.size() + 1);                  //LENGTH  IN BYTES
        memoryVector.insert(memoryVector.end(), arrayValuesVector.begin(), arrayValuesVector.end()); //ARRAY_BINARY_VALUE/S
        return memoryVector;
    }

    //TODO: IS ALMOST IDENTICAL TO DECODE IN BINARY UTILS
    NetworkObject NetStaticArray::StaticDeserialization(std::vector<uint8_t> argsMemory){
        std::vector<std::shared_ptr<INetworkType>> objectStructure;
        auto byteSequenceLength = argsMemory.size();

        for (uint32_t memPos = 0; memPos < byteSequenceLength;)
        {
            auto currentDeserializedBytes = 0;
            auto commandValue = argsMemory.at(memPos);
            auto typeCode = static_cast<MAP::NetworkType>(commandValue);
            auto currentBuffer = std::vector<uint8_t>(argsMemory.begin()+memPos,argsMemory.end());
        
            auto currentDeserializedType = SerializerTypes.Get()[m_array_content_type]->RawDeserialization(currentBuffer);

            for (auto dtype : currentDeserializedType)
            {
                currentDeserializedBytes += dtype->GetRawSize();
            }
            objectStructure.insert(objectStructure.end(), currentDeserializedType.begin(), currentDeserializedType.end());
            memPos += currentDeserializedBytes;
        }
        return objectStructure;
    }

    std::vector<std::shared_ptr<INetworkType>> NetStaticArray::RawDeserialization(std::vector<uint8_t> argsMemory)
    {
        return StaticDeserialization(argsMemory);
    }

    std::vector<std::shared_ptr<INetworkType>> NetStaticArray::Deserialize(const uint8_t *argsMemory)
    {
        std::vector<std::shared_ptr<INetworkType>> objectStructure;
        auto memoryTag = m_instance_name.Deserialize(argsMemory).at(0);
        auto memoryTagOffset = memoryTag->GetSize();
        m_array_content_type = static_cast<NetworkType>(argsMemory[memoryTagOffset + MEM_OFFSET_1]);
        auto arrayLength = argsMemory[memoryTagOffset + MEM_OFFSET_2];
        auto startOffset = memoryTagOffset + MEM_OFFSET_3;
        auto valuesMemoryVector = std::vector<uint8_t>(argsMemory + startOffset, (argsMemory + arrayLength));
        auto decodedArrayVal = StaticDeserialization(valuesMemoryVector);
        objectStructure.push_back(std::make_shared<MAP::NetStaticArray>(decodedArrayVal,m_array_content_type, memoryTag->GetName()));
        return objectStructure;
    }

    NetworkType NetStaticArray::GetType()
    {
        return NetworkType::SARRAY;
    }

    const char *NetStaticArray::GetName()
    {
        return m_instance_name.GetName();
    }

    uint32_t NetStaticArray::GetRawSize()
    {
        std::size_t partialSize = 0;
        for (auto arrayVal : m_values)
            partialSize += arrayVal->GetRawSize();
        return partialSize;
    };

    uint32_t NetStaticArray::GetSize()
    {
        return m_instance_name.GetSize() + GetRawSize() + 3; //+ 3 for type,array length and array type bytes
    }

    NetworkObject NetStaticArray::GetValues()
    {
        return m_values;
    }

    std::shared_ptr<MAP::INetworkType> NetStaticArray::At(std::size_t pos)
    {
        return m_values.at(pos);
    }

}