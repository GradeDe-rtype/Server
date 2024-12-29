//
// Created by dan13615 on 12/22/24.
//

#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "Player.hpp"
#include "RType.hpp"
#include "Utils.hpp"
#include "rfcArgParser.hpp"

namespace Server
{
    class Network
    {
        public:
            using SocketPtr = std::shared_ptr<boost::asio::ip::tcp::socket>;
            using MessageCallback = std::function<void(int clientId, const std::string &data)>;
            using DisconnectCallback = std::function<void(int clientId)>;
            using ConnectCallback = std::function<void(int clientId)>;

            struct Client {
                    int id;
                    SocketPtr socket;
                    std::array<char, 1024> buffer;
            };

            Network(boost::asio::io_context &io_context, short port);

            void start(void);
            void setMessageCallback(MessageCallback callback);
            void setDisconnectCallback(DisconnectCallback callback);
            void setConnectCallback(ConnectCallback callback);
            void unicast(int clientId, const std::string &message);
            void multicast(const std::string &message, const std::vector<int> &includeIds = {});
            void multicast_excluded(const std::string &message, const std::vector<int> &excludeIds = {});
            void broadcast(const std::string &message);
            SocketPtr getClientSocket(int clientId);

        private:
            void accept(void);
            void read(std::shared_ptr<Client> client);
            void disconnect(int clientId);
            std::shared_ptr<Client> findClient(int clientId);

            boost::asio::io_context &io_context_;
            boost::asio::ip::tcp::acceptor acceptor_;
            std::vector<std::shared_ptr<Client>> clients_;
            int next_client_id_;

            MessageCallback onMessage;
            DisconnectCallback onDisconnect;
            ConnectCallback onConnect;
    };
} // namespace Server

#endif // NETWORK_HPP
