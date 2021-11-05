#ifndef SPAWNEDENTITY_H
#define SPAWNEDENTITY_H
#include <string>

namespace MAP
{
    class SpawnedEntity
    {
    public:
        SpawnedEntity(std::string &prefabName, uint32_t playerOwner) : PrefabName(prefabName), PlayerOwner(playerOwner)
        {
        }
        ~SpawnedEntity() {}
        uint64_t id;
        std::string PrefabName;
        uint32_t PlayerOwner;
    };
} // namespace MAP

#endif