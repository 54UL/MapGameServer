#ifndef TYPESINSTANCEMANAGER_H
#define TYPESINSTANCEMANAGER_H

#include <stdint.h>
#include <memory>
#include <vector>
#include <map>
#include "./SerializerAPI.hpp"
#include "./Types/MemoryTag.hpp"
#include "./Types/Command.hpp"
#include "./Types/Byte.hpp"
#include "./Types/Array.hpp"
#include "./Types/String.hpp"
#include "./Types/Float.hpp"
#include "./Types/Integer.hpp"

namespace MAP
{
    class TypesManager
    {
    public:
        TypesManager()
        {
            Initialize();
        }
        ~TypesManager()
        {
            Clean();
        }

    public:
        void Initialize()
        {
            m_serializer_network_types.insert(std::make_pair(MAP::NetworkType::TAG, std::make_shared<MAP::MemoryTag>()));
            m_serializer_network_types.insert(std::make_pair(MAP::NetworkType::COMMAND, std::make_shared<MAP::NetCommand>()));
            m_serializer_network_types.insert(std::make_pair(MAP::NetworkType::BYTE, std::make_shared<MAP::NetByte>()));
            m_serializer_network_types.insert(std::make_pair(MAP::NetworkType::ARRAY, std::make_shared<MAP::NetArray>()));
            m_serializer_network_types.insert(std::make_pair(MAP::NetworkType::STRING, std::make_shared<MAP::NetString>()));
            m_serializer_network_types.insert(std::make_pair(MAP::NetworkType::FLOAT, std::make_shared<MAP::NetFloat>()));
            m_serializer_network_types.insert(std::make_pair(MAP::NetworkType::INT, std::make_shared<MAP::NetInt>()));
        }

        void Clean()
        {
            m_serializer_network_types.clear();
        }

        std::map<MAP::NetworkType, std::shared_ptr<INetworkType>> Get()
        {
            return m_serializer_network_types;
        }

    private:
        std::map<MAP::NetworkType, std::shared_ptr<INetworkType>> m_serializer_network_types;
    };
    static TypesManager SerializerTypes;
}
#endif