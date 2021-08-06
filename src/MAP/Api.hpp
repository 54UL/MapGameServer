#ifndef MAP_API
#define MAP_API

//ALL INCLUDES
//STD CONTAINERS
#include <map>
#include <vector>
#include <array>
#include <memory>
#include <queue>
#include <string>

#define MAX_DATA_PAYLOAD 512
#define MAX_CLIENTS 16
#define MAX_ARRAY_SIZE 1024

namespace MAP
{
    //Containers
    template <class T>
    using Vector = std::vector<T>;
    template <class T, class T1>
    using Map = std::map<T,T1>;
    template <class T, int size>
    using Array = std::array<T, MAX_ARRAY_SIZE>;
    template <class T>
    using Queue = std::queue<T>;
    using String = std::string;
    //SmartPointers
    template <class T>
    using SharedPtr = std::shared_ptr<T>;
} // namespace MAP

#endif