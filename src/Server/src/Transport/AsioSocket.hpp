#ifndef IMAP_ASIO_SOCKET_H
#define IMAP_ASIO_SOCKET_H

#include <asio.hpp>
#include <memory>
#include <spdlog/spdlog.h>

#include "../../include/IMapTransport.hpp"
#include "../../include/MapApi.hpp"

using asio::ip::udp;

namespace MAP
{
    class AsioSocket : public IMapTransport
    {
    public:
        AsioSocket()
        {
        }

        virtual ~AsioSocket()
        {
        }

        void Start(const short port) override
        {
            m_socket = std::make_unique<udp::socket>(m_io_context, port);
        }

        void Close() override
        {
        }

        void PollMessages(std::function<void(const uint8_t *bytes, std::size_t length)> callback) override
        {
            //NO POLLING ALGORITHM (just for the test case)
            m_socket->async_receive_from(
                asio::buffer(m_data, MAX_DATA_PAYLOAD), m_receiver_endpoint,
                [&](std::error_code ec, std::size_t bytes_recvd)
                {
                    spdlog::info("[INCOMIG DATA:{} bytes from {} ]", bytes_recvd, m_receiver_endpoint.address());

                    if (!ec && bytes_recvd > 0)
                    {
                        callback(m_data, bytes_recvd);
                    }
                    else
                    {
                        spdlog::error("ERROR SENDING DATA...");
                    }
                });
            m_io_context.run();
            m_io_context.reset();
        }

        void Send(const uint8_t *bytes, std::size_t length, uint32_t clientId) override
        {
            asio::ip::udp::endpoint to; //find connection by clientId
            m_socket->async_send_to(
                asio::buffer(bytes, length), to,
                [this](std::error_code err, std::size_t sended)
                {
                    //DO SOMETHING WITH THE THINGS SENDED
                    //  std::cout << "[DATA SENDED (" << sended << ")]" << std::endl;
                    spdlog::info("[DATA SENDED{}]", sended);
                });
            m_io_context.run();
            m_io_context.reset();
        }

    private:
        uint8_t m_data[MAX_DATA_PAYLOAD];
        MAP::Map<uint32_t, udp::endpoint> m_network_clients; // add model to store endpoint and port
        asio::io_context m_io_context;
        std::unique_ptr<udp::socket> m_socket;
        udp::endpoint m_receiver_endpoint;
    };
}

#endif