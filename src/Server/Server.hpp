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
#include "Command.hpp"
#include "Player.hpp"
#include "Room.hpp"
#include "RType.hpp"
#include "Utils.hpp"
#include "rfcArgParser.hpp"

/*  ---- CLASS ---- */

namespace RType::Game
{
    class Room;
}
namespace RType::Game::Entity
{
    class Player;
}
namespace Server
{
    class Command;
    class TCP
    {
        public:
            TCP(boost::asio::io_context &io_context, short port);
            void send_message(int client_id, int receiver_id, rfcArgParser::DataPacket data);
            void send_multicast(rfcArgParser::DataPacket data, const std::vector<int> &exclude = {});
            void send_broadcast(rfcArgParser::DataPacket data);
            RType::Game::Entity::Player &get_client(int client_id);
            std::shared_ptr<RType::Game::Entity::Player> &get_client_ptr(int client_id);

            ~TCP();

            bool getRunning() { return is_running; }

            void setRunning(bool running) { is_running = running; }

        private:
            void start_accept();
            void start_read(RType::Game::Entity::Player player);

            std::vector<std::shared_ptr<RType::Game::Entity::Player>> &get_clients() { return clients_; }
            void remove_client(int client_id);
            void add_client(std::shared_ptr<RType::Game::Entity::Player> client);
            bool client_exist(int client_id);

            void add_room(const RType::Game::Room room);
            void remove_room(int room_id);
            RType::Game::Room &get_room(int room_id);
            bool room_exist(int room_id);
            void add_player_to_room(int room_id, int player_id);
            void remove_player_from_room(int room_id, int player_id);

            boost::asio::ip::tcp::acceptor acceptor_;
            int next_client_id_ = 0;
            std::vector<std::shared_ptr<RType::Game::Entity::Player>> clients_;
            std::vector<std::unique_ptr<RType::Game::Room>> rooms_;
            Command *command_processor;
            bool is_running = true;
    };
} // namespace Server

#endif // SERVER_HPP
