#ifndef TRANSPORT_MANAGER_H
#define TRANSPORT_MANAGER_H

#include "./MapApi.hpp"
#include "./IMapObject.hpp"
#include "./IMapTransport.hpp"
#include "./IMapDataFormat.hpp"

namespace MAP
{
    class TransportManager
    {
    public:
        TransportManager(TransportMethod method,short port)
        {
        }
        ~TransportManager()
        {
        }

        void InitializeMethod(TransportMethod method)
        {
        }

        //use this if u want to reload the encoding method
        bool LoadMethod(TransportMethod method)
        {
            //Clean up to re-initilize the formating method (if equals to actual formating method does nothing)
        }

        std::shared_ptr<IMapTransport> GetCurrentMethod()
        {
            return this->m_current_transport_method;
        }

    private:
        std::shared_ptr<IMapTransport> m_current_transport_method;
    };
} // namespace MAP

#endif