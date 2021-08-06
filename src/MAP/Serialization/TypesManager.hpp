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
        }

    public:
        void Initialize()
        {
            
        }

        void Clean()
        {
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