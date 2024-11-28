/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }
    >> Daniil STEPANOV - { daniil.stepanov@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef SERVER_HPP
    #define SERVER_HPP

    /*  ---- INCLUDES ---- */
    #include "RType.hpp"
    #include "Command.hpp"
    #include "Utils.hpp"


    /*  ---- CLASS ---- */
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
            Server::Command command_processor;
    };
}

#endif //SERVER_HPP
