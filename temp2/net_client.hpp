#pragma once
#include "net/net_common.hpp"
#include "net/net_message.hpp"
#include "net/net_tsqueue.hpp"
#include "net/net_connection.hpp"

namespace net
{
template <typename T>
class client_interface
{
public:
        client_interface() : m_socket(m_context) {}
        virtual ~client_interface() {Disconnect();}
        bool Connect(const std::string& host, const uint16_t port)
        {
                try
                {
                        //create m_connection
                        m_connection = std::make_unique<connection<T> >();

                        asio::ip::tcp::resolver resolver(m_context);
                        auto m_endpoints = resolver.resolve(host, std::to_string(port));

                        m_connection->connectToServer(m_endpoints);

                        m_threadContext = std::thread([this]() {m_context.run();});
                }
                catch (std::exception& e)
                {
                        std::cerr << "Client Exception: " << e.what() << "\n";
                        return false;
                }
                return false;
        }

        void Disconnect()
        {
                if (IsConnected())
                {
                        m_connection->Disconnect();
                }

                //stop the asio asio context
                m_context.stop();
                if (m_threadContext.joinable())
                        m_threadContext.join();

                m_connection->Release();
        }

        bool IsConnected()
        {
                if (m_connection)
                        return m_connection->IsConnected();
                else
                        return false;
        }

        tsqueue<owned_message<T> >& Incoming()
        {
                return m_qMessagesIn;
        }
protected:
        asio::io_context m_context;
        std::thread m_threadContext;
        asio::ip::tcp::socket m_socket;
        std::unique_ptr<connection<T> > m_connection;
private:
        tsqueue<owned_message<T> > m_qMessagesIn;
};
}
