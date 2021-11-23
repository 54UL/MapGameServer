#ifndef COMANDS_MANAGER_H
#define COMANDS_MANAGER_H

#include "./MapApi.hpp"
#include "./IMapCommand.hpp"

namespace MAP
{
    class CommandsManager
    {
    public:
        CommandsManager()
        {
            
        }

        ~CommandsManager()
        {
        }

    

        void AddSet(MAP::Map<uint32_t, std::shared_ptr<IMapCommand>> commands)
        {
            m_registered_commands.insert(commands.begin(),commands.end());
        }

        MAP::Map<uint32_t, std::shared_ptr<IMapCommand>> GetDefaultSet()
        {
            return m_registered_commands;
        }

    private:
        MAP::Map<uint32_t, std::shared_ptr<IMapCommand>> m_registered_commands;
    };
} // namespace MAP

#endif