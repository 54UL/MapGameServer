#ifndef IMAP_ASIO_SOCKET_H
#define IMAP_ASIO_SOCKET_H

#include <asio.hpp>
#include <memory>
#include <spdlog/spdlog.h>

#include "./Models/AsioClientData.hpp"
#include "../../include/IMapTransport.hpp"
#include "../../include/MapApi.hpp"

using asio::ip::udp;

namespace MAP
{
    class AsioSocket : public IMapTransport
    {
    public:
        AsioSocket() : m_last_client_network_id(512)
        {
            // auto ipAddrr = asio::ip::address::from_string(IpAddress);
            // auto clientEndpoint = udp::endpoint(ipAddrr, port);
        }

        virtual ~AsioSocket()
        {
        }

        void Start(const short port) override
        {
            m_socket = std::make_unique<udp::socket>(m_io_context, udp::endpoint(udp::v4(), port));
        }

        void Close() override
        {
        }

        uint32_t GenerateClientId()
        {
            m_last_client_network_id++;
            return m_last_client_network_id;
        }

        bool KnownOrigin(const asio::ip::udp::endpoint &endpoint)
        {
            //TODO::MOVE THIS TO ANOTHER LISTENING SOCKET AND THREAD....
            auto hostEndpointVal = endpoint.address().to_v4().to_uint();
            auto findedClientId = m_network_hosts[hostEndpointVal];
            if (findedClientId == 0)
            {
                auto newClientId = GenerateClientId();
                m_network_clients.insert(std::make_pair(newClientId, endpoint));
                m_network_hosts[hostEndpointVal] = newClientId;
            }
            return findedClientId != 0;
        }

        void PollMessages(std::function<void(uint8_t *bytes, std::size_t length)> callback) override
        {
            //TODO:NO POLLING AND DISPATCHING ALGORITHM (TODO)
            //TODO:THIS HAS TO BE IN A THREAD
            m_socket->async_receive_from(
                asio::buffer(m_data, MAX_DATA_PAYLOAD), m_receiver_endpoint,
                [&](std::error_code ec, std::size_t bytes_recvd)
                {
                    spdlog::info("[INCOMIG DATA: bytes from {} ]", m_receiver_endpoint.address().to_string());
                    if (KnownOrigin(m_receiver_endpoint))
                    {
                        spdlog::warn("User had no server id,but was created...");
                    }

                    if (!ec && bytes_recvd > 0)
                    {
                        callback(m_data, bytes_recvd);
                    }
                    else
                    {
                        spdlog::error("ERROR SENDING DATA...");
                    }
                });
            m_io_context.reset();
            m_io_context.run();
        }

        void Send(uint8_t *bytes, std::size_t length, uint32_t clientId) override
        {
            auto to = m_network_clients[clientId];
            //TODO: INSERT HERE A thread worker like algorithm to dispatch async sends??...idk
            m_socket->async_send_to(
                asio::buffer(bytes, length), to,
                [this](std::error_code err, std::size_t sended)
                {
                    //DO SOMETHING WITH THE THINGS SENDED
                    // std::cout << "[DATA SENDED (" << sended << ")]" << std::endl;
                    spdlog::info("[DATA SENDED{}]", sended);
                });
            m_io_context.reset();
            m_io_context.run();
        }

    private:
        uint8_t m_data[MAX_DATA_PAYLOAD];
        uint32_t m_last_client_network_id;
        MAP::Map<uint32_t, udp::endpoint> m_network_clients; //Known hosts
        MAP::Map<uint32_t, uint32_t> m_network_hosts;        //Hosts lookup (just for fast fetch)
        asio::io_context m_io_context;
        udp::endpoint m_receiver_endpoint;
        std::unique_ptr<udp::socket> m_socket;
    };
}

#endif