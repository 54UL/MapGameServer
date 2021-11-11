#ifndef PAYLOADFORMAT_H
#define PAYLOADFORMAT_H
#include "../../include/IMapNetPayload.hpp"

namespace MAP{
    class PayLoadFormat{
        public:
        PayLoadFormat();
        virtual ~PayLoadFormat();
        //initialize()
        //getDefault()
        //IMapNetPayLoad get(id) EJ: JSON,MAP_SERIAL,PROTOBUFF(NOT GONNA IMPLEMENT THAT)
    };
}

#endif