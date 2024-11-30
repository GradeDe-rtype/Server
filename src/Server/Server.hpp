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
    #include <RType.hpp>
    #include <Utils.hpp>
    #include <Command.hpp>

    /*  ---- CLASS ---- */



namespace Server
{
    class Command;
    class TCP
    {
        public:
            TCP(boost::asio::io_context& io_context, short port);
            void send_message(int client_id, int receiver_id, const std::string& message);
            std::unordered_map<int, std::shared_ptr<boost::asio::ip::tcp::socket>> get_clients(void) {
                return clients_;
            }

            ~TCP();

        private:
            void start_accept();
            void start_read(int client_id);
            boost::asio::ip::tcp::acceptor acceptor_;
            std::unordered_map<int, std::shared_ptr<boost::asio::ip::tcp::socket>> clients_;
            int next_client_id_ = 0;
            Command* command_processor;
    };
}

#endif //SERVER_HPP
