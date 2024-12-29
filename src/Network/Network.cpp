//
// Created by dan13615 on 12/22/24.
//

#include "Network.hpp"

namespace Server
{

    Network::Network(boost::asio::io_context &io_context, short port)
        : io_context_(io_context),
          acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
          next_client_id_(0) {}

    void Network::start(void)
    {
        accept();
    }

    void Network::accept(void)
    {
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(io_context_);
        acceptor_.async_accept(
            *socket,
            [this, socket](const boost::system::error_code &error) {
                if (!error) {
                    auto client = std::make_shared<Client>();
                    client->id = next_client_id_++;
                    client->socket = socket;
                    clients_.push_back(client);
                    if (onConnect) {
                        onConnect(client->id);
                    }
                    read(client);
                }
                accept();
            });
    }

    void Network::read(std::shared_ptr<Client> client)
    {
        client->socket->async_read_some(
            boost::asio::buffer(client->buffer),
            [this, client](const boost::system::error_code &error, std::size_t length) {
                if (!error) {
                    if (onMessage) {
                        std::string message(client->buffer.data(), length);
                        onMessage(client->id, message);
                    }
                    read(client);
                } else {
                    disconnect(client->id);
                }
            });
    }

    Network::SocketPtr Network::getClientSocket(int clientId)
    {
        auto client = findClient(clientId);
        return client ? client->socket : nullptr;
    }

    void Network::setMessageCallback(MessageCallback callback)
    {
        onMessage = std::move(callback);
    }

    void Network::setDisconnectCallback(DisconnectCallback callback)
    {
        onDisconnect = std::move(callback);
    }

    void Network::setConnectCallback(ConnectCallback callback)
    {
        onConnect = std::move(callback);
    }

    void Network::disconnect(int clientId)
    {
        if (onDisconnect) {
            onDisconnect(clientId);
        }
        clients_.erase(
            std::remove_if(
                clients_.begin(),
                clients_.end(),
                [clientId](const auto &client) { return client->id == clientId; }),
            clients_.end());
    }

    std::shared_ptr<Network::Client> Network::findClient(int clientId)
    {
        auto it = std::find_if(
            clients_.begin(),
            clients_.end(),
            [clientId](const auto &client) { return client->id == clientId; });
        return it != clients_.end() ? *it : nullptr;
    }

    void Network::unicast(int clientId, const std::string &message)
    {
        auto client = findClient(clientId);
        if (client) {
            boost::asio::async_write(
                *client->socket,
                boost::asio::buffer(message),
                [this, clientId](const boost::system::error_code &error, std::size_t /*length*/) {
                    if (error) {
                        disconnect(clientId);
                    }
                });
        }
    }

    void Network::multicast(const std::string &message, const std::vector<int> &includeIds)
    {
        for (const auto &client : clients_) {
            if (std::find(includeIds.begin(), includeIds.end(), client->id) != includeIds.end()) {
                unicast(client->id, message);
            }
        }
    }

    void Network::multicast_excluded(const std::string &message, const std::vector<int> &excludeIds)
    {
        for (const auto &client : clients_) {
            if (std::find(excludeIds.begin(), excludeIds.end(), client->id) == excludeIds.end()) {
                unicast(client->id, message);
            }
        }
    }

    void Network::broadcast(const std::string &message)
    {
        for (const auto &client : clients_) {
            unicast(client->id, message);
        }
    }
}; // namespace Server