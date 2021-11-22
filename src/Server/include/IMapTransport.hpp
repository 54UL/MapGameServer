#ifndef IMAP_TRANSPORT_H
#define IMAP_TRANSPORT_H
#include <functional>

namespace MAP
{
    class IMapTransport
    {
    public:
        IMapTransport();
        virtual ~IMapTransport();

        virtual void Start(const short port) = 0;
        virtual void Close() = 0;
        virtual void PollMessages(std::function<void(const uint8_t *bytes, std::size_t length)> callback) = 0;
        virtual void Send(const uint8_t *bytes, std::size_t length, uint32_t clientId) = 0;
    };
}

#endif