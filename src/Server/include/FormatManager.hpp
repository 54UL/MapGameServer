#ifndef FORMAT_MANAGER_H
#define FORMAT_MANAGER_H

#include "./IMapDataFormat.hpp"
#include "./MapApi.hpp"
#include "./IMapObject.hpp"

namespace MAP
{
    class FormatManager
    {
    public:
        FormatManager(EncodingMethod method) {

        }
        ~FormatManager()
        {

        }

        void InitializeMethod(EncodingMethod method)
        {

        }
        
        //use this if u want to reload the encoding method
        bool LoadMethod(EncodingMethod method){
            //Clean up to re-initilize the formating method (if equals to actual formating method does nothing)
        }

        std::shared_ptr<IMapDataFormat> GetCurrentMethod(){
            return this->m_current_data_format;
        }

    private:
        std::shared_ptr<IMapDataFormat> m_current_data_format;
    };
} // namespace MAP

#endif