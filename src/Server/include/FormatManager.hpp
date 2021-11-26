#ifndef FORMAT_MANAGER_H
#define FORMAT_MANAGER_H

#include "./IMapDataFormat.hpp"
#include "./MapApi.hpp"
#include "./IMapObject.hpp"
#include "../src/Serialization/BinaryFormat.hpp"
#include "../src/Serialization/JsonFormat.hpp"

namespace MAP
{
    class FormatManager
    {
    public:
        FormatManager(EncodingMethod method)
        {
            LoadMethod(method);
        }
        ~FormatManager()
        {
        }

        //use this if u want to reload the encoding method
        bool LoadMethod(EncodingMethod method)
        {
            if (m_current_data_format == nullptr)
                m_current_data_format.reset();

            switch (method)
            {
            case EncodingMethod::MAP_BINARY:
                this->m_current_data_format = std::make_shared<MAP::BinaryFormat>();
                break;
            case EncodingMethod::NLOHMANN_JSON:
                this->m_current_data_format = std::make_shared<MAP::MapJsonFormat>();
                break;
            }

            return m_current_data_format != nullptr;
        }

        std::shared_ptr<IMapDataFormat> GetCurrentMethod()
        {
            return this->m_current_data_format;
        }

    private:
        std::shared_ptr<IMapDataFormat> m_current_data_format;
    };
} // namespace MAP

#endif