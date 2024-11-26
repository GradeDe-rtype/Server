//
// Created by dan13615 on 11/26/24.
//

#ifndef SERVER_HPP
    #define SERVER_HPP

    #include <boost/asio.hpp>
    #include <iostream>
    #include <memory>
    #include <string>
    #include <unordered_map>

namespace Server
{
    class TCP
    {
        public:
            TCP(boost::asio::io_context& io_context, short port);
        private:
            void start_accept();
            void start_read(int client_id);
            void send_message(int client_id, const std::string& message);
            boost::asio::ip::tcp::acceptor acceptor_;
            std::unordered_map<int, std::shared_ptr<boost::asio::ip::tcp::socket>> clients_;
            int next_client_id_ = 0;
    };
}

#endif //SERVER_HPP
