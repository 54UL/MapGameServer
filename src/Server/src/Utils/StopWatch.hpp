#ifndef STOP_WATCH_H
#define STOP_WATCH_H

#include <asio.hpp>

using asio::ip::udp;

//todo: everything 
namespace MAP
{
    class StopWatch
    {
    public:
        StopWatch()
        {
        }
        ~StopWatch()
        {
        }

        //BENCHMARKING FUNCTIONS UTILS  GOES HERE
    };

    static StopWatch MapStopWatch;
}

#endif