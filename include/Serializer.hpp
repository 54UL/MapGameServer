#ifndef SERIALIZER_H
#define SERIALIZER_H
#include <stdint.h>
#include <memory>
#include <vector>
#include <map>

namespace MAP
{
    //TODO:
    //DESHARDCODEAR TIPOS =)

    enum class NetworkType : uint8_t
    {
        COMMAND = 0x01,
        STRING = 0x02,
        FLOAT = 0x03,
        ARRAY = 0x04,
        BYTE = 0x05,
        INT = 0x06,
        UINT = 0x07,
        OBJECT = 0x08,
    };

    class INetworkType
    {
    public:
        INetworkType() {}
        virtual ~INetworkType() {}
        // virtual bool TrySerialize(std::vector<uint8_t> memoryVector) = 0;
        virtual bool TrySerialize(Serializer *serializer) = 0;

        // virtual std::vector<std::shared_ptr<INetworkType>> Deserialize(std::vector<uint8_t> payloadData, uint16_t lastPayloadPosition) = 0;
        virtual std::vector<std::shared_ptr<INetworkType>> Deserialize(Serializer *serializer) = 0;
        virtual NetworkType GetType() = 0;
        virtual uint32_t GetSize() = 0;
    };

    class Command : public INetworkType
    {
    public:
        Command() : m_command_id(0)
        {
        }

        Command(uint8_t commandId) : m_command_id(commandId)
        {
        }

        ~Command()
        {
        }

        bool TrySerialize(Serializer *serializer) override
        {
            std::vector<uint8_t> serializedArgsStack;
            serializedArgsStack.push_back((uint8_t)GetType()); // TYPE CODE
            serializedArgsStack.push_back(m_command_id);
            serializer->AddMemoryCounter(serializedArgsStack.size()); // type offset
            serializer->PushArgs(serializedArgsStack);
            return true;
        }

        std::vector<std::shared_ptr<INetworkType>> Deserialize(Serializer *serializer) override
        {
            auto args = serializer->GetCurrentPayLoad();
            std::vector<std::shared_ptr<INetworkType>> objectStructure;
            objectStructure.push_back(std::make_shared<Command>(args.at(0)));
            return objectStructure;
        }

        NetworkType GetType() override
        {
            return NetworkType::COMMAND;
        }

        uint32_t GetSize() override
        {
            return sizeof(m_command_id);
        }

    private:
        uint8_t m_command_id;
    };

    class Byte : public INetworkType
    {
    public:
        Byte() : m_value(0)
        {
        }
        Byte(uint8_t value) : m_value(value)
        {
        }
        ~Byte()
        {
        }

        bool TrySerialize(Serializer *serializer) override
        {
            std::vector<uint8_t> serializedArgsStack;
            serializedArgsStack.push_back((uint8_t)GetType()); // TYPE CODE
            serializedArgsStack.push_back(m_value);
            serializer->AddMemoryCounter(serializedArgsStack.size()); // type offset
            serializer->PushArgs(serializedArgsStack);
            return true;
        }

        std::vector<std::shared_ptr<INetworkType>> Deserialize(Serializer *serializer) override
        {
            auto args = 
            std::vector<std::shared_ptr<INetworkType>> objectStructure;
            objectStructure.push_back(std::make_shared<Byte>(args.at(0)));
            return objectStructure;
        }

        NetworkType GetType() override
        {
            return NetworkType::BYTE;
        }

    private:
        uint8_t m_value;
    };

    class Serializer
    {
    public:
        Serializer();
        ~Serializer();

    private:
        void Initialize()
        {
            RegisterCoreTypes();
            InitializeMembers();
        }

        inline void RegisterCoreTypes()
        {
        }

        inline void InitializeMembers()
        {
        }

        inline void SetMemoryBuffer(const uint8_t *bytes){
            for (auto b:bytes){
                m_raw_memory_packet.push_back(b);
            }
        }

    public:
        std::vector<uint8_t> SerializeDataSequence(std::vector<std::shared_ptr<INetworkType>> sequence)
        {
            //Evalua toda la sequencia y retorna el valor de memoria puro.
        }

        std::vector<std::shared_ptr<INetworkType>> DeserializeDataSequence(const uint8_t *bytes)
        {
            std::vector<std::shared_ptr<INetworkType>> objectStructure;
            //Conseguir posicion actual **
            //Conseguir el payload dado al comando actual desde lastPos hacia currentCommandOffset
            //Indicar la siguiente posicion de inicio...
            // [COMMAND,SUBSCRIBE,STRING,STRING,INT]
            //[0x01,0x01,0x50]
            auto byteSequenceLength = sizeof(bytes);
            for (uint32_t memPos = 0; memPos < byteSequenceLength;)
            {
                auto memoryValue = bytes[memPos];
                auto typeCode = static_cast<NetworkType>(memoryValue);

                if (memoryValue == 0)
                    return std::vector<std::shared_ptr<INetworkType>>();
                m_current_processing_type = typeCode;
                //TYPES MUST BE FETCHED (KNOW THE CURRENT TYPE AND READS HIS LENGTHS)
                auto currentDeserializedType = m_serializer_network_types[typeCode]->Deserialize(this);
                // objectStructure.insert(objectStructure.end(), currentDeserializedType);
                memPos = m_current_memory_position;
            }
            return objectStructure;
        }

        //Push all args of an sequence into the memory stack
        bool PushArgs(std::vector<uint8_t> stackValues)
        {
        }

        //Pops all args pushed by an sequence
        std::vector<uint8_t> PopArgs()
        {
            return std::vector<uint8_t>();
        }

        void AddMemoryCounter(uint32_t memoryPosition)
        {
            m_current_memory_position += memoryPosition;
        }

        void DeterminePayLoad(const INetworkType *type)
        {
        }

        //Returns an vector from the current position
        std::vector<uint8_t> GetCurrentPayLoad()
        {
            return std::vector<uint8_t>();
        }

    private:
        NetworkType m_current_processing_type;
        uint32_t m_current_memory_position = 0;//
        std::vector<uint8_t> m_raw_memory_packet;//Current memory packet
        std::vector<std::shared_ptr<INetworkType>> m_current_memory_packet; //A complete comannd memory block ej : [COMMAND,SUBSCRIBE,STRING,STRING,INT]
        std::map<NetworkType, std::shared_ptr<INetworkType>> m_serializer_network_types; //Registered types
    };
}

#endif