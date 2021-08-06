
#ifndef MEMORY_TAG_H
#define MEMORY_TAG_H

#include "../BinaryObject.hpp"
#include "../SerializerAPI.hpp"
#include <string>

namespace MAP
{
    class MemoryTag : public INetworkType
    {
    public:
        MemoryTag() : instance_name_(std::string("NONE"))
        {
        }

        MemoryTag(const std::string &name) : instance_name_(name)
        {
        }

        ~MemoryTag()
        {
        }

        std::vector<uint8_t> TrySerialize() override
        {
            std::vector<uint8_t> memoryVector;
            memoryVector.push_back(static_cast<uint8_t>(GetType()));
            memoryVector.push_back(static_cast<uint8_t>(instance_name_.length())); //CHANGE THIS CAST of size_t by uint8_t
            for (auto stringIterator : instance_name_)
            {
                memoryVector.push_back(stringIterator);
            }
            return memoryVector;
        }

        //TODO:CONTINUAR IMPLEMENTANDO EL DESERIALIZADO PARA TODOS LOS TIPOS
        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override
        {
            uint8_t tagLength = argsMemory[0]; //lenght first pos
            std::string instanceName;
            for (uint8_t i = 1; i < tagLength; i++)
            {
                instanceName.push_back(i);
            }
            std::vector<std::shared_ptr<INetworkType>> objectStructure;
            objectStructure.push_back(std::make_shared<MAP::MemoryTag>(instanceName));
            return objectStructure;
        }

        NetworkType GetType() override
        {
            return NetworkType::TAG;
        }

        const char *GetName() override
        {
            return instance_name_.c_str();
        }

        uint32_t GetSize() override
        {
            return instance_name_.length() + 2; //1 byte for the type and other for the tag string length
        }

    private:
        std::string instance_name_;
    };
}

#endif