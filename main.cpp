#include "includes/server.hpp"

namespace Server {
    // Starts accepting a new connection
    void TcpServer::start_accept() {
        socket_ = std::make_shared< boost::asio::ip::tcp::socket>(acceptor_.get_io_service());
        acceptor_.async_accept(*socket_,
            boost::bind(&TcpServer::handle_accept, this, boost::asio::placeholders::error));
    }

    // Handles an accepted connection
    void TcpServer::handle_accept(const boost::system::error_code& error) {
        if (!error) {
            // Assign a new client ID and display a welcome message
            int client_id = next_client_id_++;
            std::cout << "Client " << client_id << " connected." << std::endl;
            // Send a welcome message to the client
            std::string welcome_message = "Welcome, Client " + std::to_string(client_id) + "!\n";
            boost::asio::write(*socket_, boost::asio::buffer(welcome_message));
            // Start reading data from the client
            start_read(client_id);
        } else {
            std::cerr << "Accept failed: " << error.message() << std::endl;
        }
        // Continue accepting new connections
        start_accept();
    }

    // Starts reading data from the connected client
    void TcpServer::start_read(int client_id) {
        // Create a buffer for reading
        auto data = std::make_shared<std::string>();
        // Read until a newline character is encountered
        async_read_until(*socket_, boost::asio::dynamic_buffer(*data), '\n',
            [this, client_id, data](const boost::system::error_code& error, std::size_t bytes_transferred) {
                if (!error) {
                    // Display the message with the client ID
                    std::cout << "Client " << client_id << " says: " << *data << std::endl;
                    data->clear();
                    start_read(client_id);
                } else {
                    std::cerr << "Read failed: " << error.message() << std::endl;
                }
            });
    }
}

int main() {
    try {
        boost::asio::io_service io_service;
        Server::TcpServer server(io_service, 12345);  // Listen on port 12345
        io_service.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
