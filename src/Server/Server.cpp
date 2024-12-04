/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }
    >> Daniil STEPANOV - { daniil.stepanov@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

/*  ---- INCLUDES ---- */
#include <Server.hpp>

namespace Server {
    TCP::TCP(boost::asio::io_context& io_context, const short port)
    : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    command_processor(new Command(*this))
    {
        start_accept();
    }

    TCP::~TCP() {
        delete command_processor;
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
                send_broadcast("connect {player_id:" + std::to_string(client_id) + "}", {client_id});
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
            std::string raw_message = buffer->substr(0, length);
            buffer->erase(0, length);
            std::string sanitized_message = RType::Utils::trim(raw_message);
            std::cout << "Client " << client_id << " says: " << sanitized_message << "\n";
            command_processor->process_command(client_id, sanitized_message);
            start_read(client_id); // Continue reading
        } else {
            std::cerr << "Client " << client_id << " disconnected.\n";
            clients_.erase(client_id);
            send_broadcast("disconnect " + std::to_string(client_id), {client_id});
        }
    });
    }

    void TCP::send_message(int client_id, int receiver_id, const std::string& message) {
        for (const auto& [id, socket] : clients_) {
            if (id == receiver_id) {
                std::string featured_message = "Client " + std::to_string(client_id) + " says: " + message + "\n";
                boost::asio::async_write(*socket, boost::asio::buffer(featured_message),
                    [](const boost::system::error_code&, std::size_t) {});
                return;
            }
        }
    }

    void TCP::send_broadcast(const std::string& message, const std::vector<int>& excluded_clients) {
        for (const auto& [id, socket] : clients_) {
            if (std::find(excluded_clients.begin(), excluded_clients.end(), id) != excluded_clients.end())
                continue;
            std::string featured_message = message + "\n";
            boost::asio::async_write(*socket, boost::asio::buffer(featured_message),
                [](const boost::system::error_code&, std::size_t) {});
        }
    }
}