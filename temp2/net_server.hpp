#pragma once

#include "net/net_common.hpp"
#include "net/net_tsqueue.hpp"
#include "net/net_message.hpp"
#include "net/net_connection.hpp"

namespace net
{
class server_interface
{
        server_interface(uint16_t port)
                : m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
        {
        }
        virtual ~server_interface()
        {
                Stop();
        }

        bool Start()
        {
                try
                {
                        WaitForClientConnection();
                        m_threadContext = std::thread([this]() {m_context.run();});
                }
                catch (std::exception& e)
                {
                        std::cerr << "[Server] Exception: " << e.what() << "\n";
                        return false;
                }
                std::cout << "[Server] Started!" << std::endl;
        }
        void Stop()
        {
                m_asioContext.stop();

                if (m_threadContext.joinable())
                        m_threadContext.join();

                std::cout << "[Server] Stopped!" << std::endl;
        }

        //ASYNC
        void WaitForClientConnection()
        {
                m_asioAcceptor.async_accept(
                        [this](std::error_code ec, asio::ip::tcp::socket socket)
                        {
                                if (!ec)
                                {
                                        std::cout << "[Server] New Connection: " << socket.remote_endpoint() << std::endl;
                                        std::shared_ptr<connection<T> > new_connection =
                                                std::make_shared<connection<T> >(connection<T>::owner::server, m_asioContext, std::move(socket), m_qMessagesIn);

                                        if (OnClientConnect(new_connection))
                                        {
                                                m_deqConnections.push_back(std::move(new_connection));
                                                m_deqConnections.back()->ConnectToClient(nIDCounter++);
                                                std::cout << "[" << m_deqConnections.back()->GetID() << "] Connection Approved: " << std::endl;
                                        }
                                        else
                                        {
                                                std::cout << "[Server] Connection Denied" << std::endl;
                                        }
                                }
                                else
                                {
                                        std::cout << "[Server] New Connection Error: " << ec.message() << std::endl;
                                }
                                WaitForClientConnection();
                        });
        }

        void MessageClient(std::shared_ptr<connection<T> > client, const message<T>& msg)
        {
                if (client && client->IsConnected())
                {
                        client->Send(msg);
                }
                else
                {
                        //should probably wait a bit to see if it's just packet loss but im just going to disconnect them for now
                        OnClientDisconnect(client);
                        client.reset();
                        m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
                }
        }
        void MessageAllClients(const message<T>& msg, std::shared_ptr<connection<T> > ignored_client = nullptr)
        {
                bool bInvalidClientExists = false;
                for (auto& client : m_deqConnections)
                {
                        if (client && client->IsConnected())
                        {
                                if (client != ignored_client)
                                        client->Send(msg);
                        }
                        else
                        {
                                OnClientDisconnect(client);
                                client.reset();
                                bInvalidClientExists = true;
                        }
                }
                if (bInvalidClientExists)
                {
                        m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr), m_deqConnections.end());
                }
        }

        void Update(size_t nMaxMessages = -1)
        {
                size_t nMesssageCount = 0;
                while (nMesssageCount < nMaxMessages && !m_qMessagesIn.empty())
                {
                        auto msg = m_qMessagesIn.pop_front();
                        OnMessage(msg.remote, msg.msg);
                        nMesssageCount++;
                }
        }

protected:
        virtual bool OnClientConnect(std::shared_ptr<connection<T> > client)
        {
                return false;
        }
        virtual void OnClientDisconnect(std::shared_ptr<connection<T> > client) {}
        virtual void OnMessage(std::shared_ptr<connection<T> > client, message<T>& msg) {}

        tsqueue<owned_message<T> > m_qMessagesIn;

        std::deque<std::shared_ptr<connection<T> > > m_deqConnections;

        asio::io_context m_context;
        std::thread m_threadContext;

        asio::ip::tcp::acceptor m_asioAcceptor;

        uint32_t nIDCounter = 10;
};
}
