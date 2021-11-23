#ifndef SPAWNEDENTITY_H
#define SPAWNEDENTITY_H
#include <string>

namespace MAP
{
    class SpawnedEntity
    {
    public:
        SpawnedEntity(std::string &prefabName, int32_t playerOwner) : PrefabName(prefabName), PlayerOwner(playerOwner)
        {
        }
        ~SpawnedEntity() {}
        int32_t id;
        int32_t PlayerOwner;
        std::string PrefabName;
    };
} // namespace MAP

#endif