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
            m_serializer_network_types.insert(std::make_pair(MAP::NetworkType::COMMAND, std::make_shared<MAP::CommandType>()));
            m_serializer_network_types.insert(std::make_pair(MAP::NetworkType::BYTE, std::make_shared<MAP::Byte>()));
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
}
#endif