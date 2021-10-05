#ifndef UNIT_TESTING
#define UNIT_TESTING
#include <memory>
#include <vector>

namespace MAP
{
    class IMapTest
    {

    public:
        IMapTest()
        {
        }
        virtual ~IMapTest()
        {
        }
        virtual void OnInit() = 0;
        virtual bool Check() = 0;
    };

    class MapTester
    {

    public:
        MapTester()
        {
        }
        ~MapTester()
        {
        }

    public:
        void Init()
        {
        }

        template <class T>
        static bool AddTest()
        {
            auto instance = std::make_shared<T>();
            m_registered_tests.push_back(instance);
            return !!instance;
        }

        static bool RunAll()
        {
            for (auto &test : m_registered_tests)
            {
                if (!test->Check())
                {
                    return false;
                }
            }
            return true;
        }

    private:
        static std::vector<std::shared_ptr<IMapTest>> m_registered_tests;
    }; // MAP
    std::vector<std::shared_ptr<IMapTest>> MAP::MapTester::m_registered_tests = std::vector<std::shared_ptr<IMapTest>>();
}
#endif