
#ifndef MEMORY_TAG_H
#define MEMORY_TAG_H

#include "../BinaryObject.hpp"
#include "../SerializerAPI.hpp"
#include <string>

namespace MAP
{
    //Internal serializer class, don't use outside
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
            std::vector<uint8_t> memoryVector(0);
            //NOTE: MEMORY TAG DOES NOT HAVE TO PASS HIS TYPE ID (ASSUMPTION DUE TO COMPOSITION)
        
            memoryVector.push_back(static_cast<uint8_t>(instance_name_.length())); //CHANGE THIS CAST of size_t by uint8_t
            for (auto stringIterator : instance_name_)
            {
                memoryVector.push_back(stringIterator);
            }
            return memoryVector;
        }

        std::vector<std::shared_ptr<INetworkType>> Deserialize(const uint8_t *argsMemory) override
        {
            uint8_t tagLength = argsMemory[1]; //lenght first pos
            std::string instanceName = "";
            for (uint8_t i = 0; i < tagLength; i++)
            {
                instanceName.push_back(argsMemory[2+ i]);
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
            return instance_name_.length() + 1;  //1 byte extra for the size byte
        }

    private:
        std::string instance_name_;
    };
}

#endif