#pragma once

#include "net/net_common.hpp"
#include "net/net_tsqueue.hpp"
#include "net/net_message.hpp"

namespace net
{
template <typename T>
class connection : public std::enable_shared_from_this<connection<T> >
{
public:
        enum class owner
        {
                server,
                client
        }
        connection (owner parent, asio::io_context& asioContext, asio::ip::tcp::socket socket tsqueue<owned_message<T> >& qIn)
                : m_asioContext(asioContext), m_socket(std::move(socket)), m_qMessagesIn(qIn)
        {
                m_nOwnerType = parent;
        }
        virtual ~connection () {}

        uint32_t GetID() const
        {
                return id;
        }

        void ConnectToClient(uint32_t uid = 0)
        {
                if (m_nOwnerType == owner::server)
                {
                        if (m_socket.is_open())
                        {
                                id = uid;
                        }
                }
        }

        void ConnectToServer();
        bool Disconnect();
        bool IsConnected() const
        {
                return m_socket.is_open();
        }

        bool Send(const message<T>& msg);
private:

        void WriteHeader()
        {
        }

        void WriteBody()
        {
        }

        void ReadHeader()
        {
                asio::async_read(m_socket, asio::buffer(&m_msgTemporaryIn.header, sizeof(message_header<T>)),
                        [this](std::error_code ec, std::size_t length) {
                                if (!ec)
                                {
                                        if (m_msgTemporaryIn.header.size > 0)
                                        {
                                                m_msgTemporaryIn.body.resize(m_msgTemporaryIn.header.size);
                                                ReadBody();
                                        }
                                        else
                                        {
                                                AddToIncomingMessageQueue();
                                        }
                                }
                                else
                                {
                                        std::cout << "[" << id << "] Read Header Failed" << std::endl;
                                        m_socket.close();
                                }
                        });
        }

        void ReadBody()
        {
                asio::async_read(m_socket, asio::buffer(&m_msgTemporaryIn.body.data(), m_msgTemporaryIn.body.size()),
                        [this](std::error_code ec, std::size_t length)
                        {
                                if (!ec)
                                {
                                        AddToIncomingMessageQueue();
                                }
                                else
                                {
                                        std::cout << "[" << id << "] Read Body Fail.\n";
                                        m_socket.close();
                                }
                        });
        }

        // Once a full message is received, add it to the incoming queue
        void AddToIncomingMessageQueue()
        {
                // Shove it in queue, converting it to an "owned message", by initialising
                // with the a shared pointer from this connection object
                if (m_nOwnerType == owner::server)
                        m_qMessagesIn.push_back({this->shared_from_this(), m_msgTemporaryIn});
                else
                        m_qMessagesIn.push_back({nullptr, m_msgTemporaryIn});

                // We must now prime the asio context to receive the next message. It
                // wil just sit and wait for bytes to arrive, and the message construction
                // process repeats itself. Clever huh?
                ReadHeader();
        }
protected:
        asio::ip::tcp::socket m_socket;

        asio::io_context& m_asioContext;

        //messages to be sent
        tsqueue<message<T> > m_qMessagesOut;

        //incoming messages
        tsqueue<owned_message<T> >& m_qMessagesIn;
        message<T> m_msgTemporaryIn;

        owner m_nOwnerType = owner::server;
        uint32_t id = 0;
};
}
