//
// Created by dan13615 on 11/26/24.
//

#include "includes/server.hpp"

int main(void) {
    try {
        boost::asio::io_context io_context;
        Server::TCP server(io_context, 8080);
        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}