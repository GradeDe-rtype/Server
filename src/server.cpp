//
// Created by dan13615 on 11/26/24.
//

#include "../includes/server.hpp"

namespace Server {

    TCP::TCP(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
        start_accept();
    }

    void TCP::start_accept() {
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(acceptor_.get_executor());
        acceptor_.async_accept(*socket, [this, socket](const boost::system::error_code& error) {
            if (!error) {
                const int client_id = next_client_id_++;
                clients_[client_id] = socket;
                std::cout << "Client " << client_id << " connected.\n";

                std::string welcome_message = "Welcome, Client " + std::to_string(client_id) + "!\n";
                boost::asio::async_write(*socket, boost::asio::buffer(welcome_message),
                    [](const boost::system::error_code&, std::size_t) {});

                start_read(client_id);
            } else {
                std::cerr << "Accept error: " << error.message() << std::endl;
            }
            start_accept(); // Accept the next connection
        });
    }

    void TCP::start_read(int client_id) {
        const auto socket = clients_[client_id];
        auto buffer = std::make_shared<std::string>(1024, '\0');

        boost::asio::async_read_until(*socket, boost::asio::dynamic_buffer(*buffer), '\n',
            [this, client_id, buffer](const boost::system::error_code& error, const std::size_t length) {
                if (!error) {
                    const std::string message = buffer->substr(0, length);
                    std::cout << "Client " << client_id << " says: " << message;
                    send_message(client_id, message);
                    start_read(client_id); // Continue reading
                } else {
                    std::cerr << "Client " << client_id << " disconnected.\n";
                    clients_.erase(client_id);
                }
            });
    }

    void TCP::send_message(int sender_id, const std::string& message) {
        for (const auto& [client_id, socket] : clients_) {
            if (client_id != sender_id) {
                std::string forward_message = "Client " + std::to_string(sender_id) + ": " + message;
                boost::asio::async_write(*socket, boost::asio::buffer(forward_message),
                    [](const boost::system::error_code&, std::size_t) {});
            }
        }
    }
}