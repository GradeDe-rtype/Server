//
// Created by dan13615 on 11/19/24.
//

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <memory>
#include <vector>


namespace Server
{
    class TcpServer {

    public:
        TcpServer(boost::asio::io_service& io_service, short port)
        : acceptor_(io_service,  boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), port)),
          socket_(std::make_shared<boost::asio::ip::tcp::socket>(io_service)), next_client_id_(0) {
            start_accept();
        }

    private:
        void start_accept();
        void handle_accept(const boost::system::error_code& error);
        void start_read(int client_id);


        boost::asio::ip::tcp::acceptor acceptor_;        // Acceptor for accepting incoming connections
        std::shared_ptr< boost::asio::ip::tcp::socket> socket_;            // Socket for the current client
        std::string data_;              // Data buffer for reading messages
        int next_client_id_;            // ID for the next client
    };
}

#endif //SERVER_HPP
