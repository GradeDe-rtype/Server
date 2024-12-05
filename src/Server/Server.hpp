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
    #include "rfcArgParser.hpp"

    /*  ---- CLASS ---- */

namespace Server
{
    class Command;
    class Player;
    class TCP
    {
        public:
            TCP(boost::asio::io_context& io_context, short port);
            void send_message(int client_id, int receiver_id, const std::string& message);
            void send_broadcast(const std::string& message, const std::vector<int>& excluded_clients = {});
            Player& get_player(int client_id);
            void remove_player(int client_id);
            std::vector<Player>& get_players() { return players_; };
            bool player_exists(int client_id);

            ~TCP();

            bool getRunning() {
                return is_running;
            }

            void setRunning(bool running) {
                is_running = running;
            }

        private:
            void start_accept();
            void start_read(Player player);
            boost::asio::ip::tcp::acceptor acceptor_;
            int next_client_id_ = 0;
            std::vector<Player> players_;
            Command* command_processor;
            bool is_running = true;
    };
}

#endif //SERVER_HPP
