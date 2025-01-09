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
#include <memory>
#include <vector>
#include "Command.hpp"
#include "Network.hpp"
#include "Player.hpp"
#include "RType.hpp"
#include "Room.hpp"
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
    class Network;
    class GameServer
    {
        public:
            GameServer(boost::asio::io_context &io_context, short port);

            void remove_client(int client_id);
            const RType::Game::Entity::Player& get_client(int client_id);
            std::shared_ptr<RType::Game::Entity::Player> &get_client_ptr(int client_id);
            bool client_exist(int client_id);
            void add_room(int id, const std::string &name);
            void start_room(size_t index) const;
            void remove_room(int room_id);
            void add_player_to_room(int room_id, int player_id);
            void remove_player_from_room(int room_id, int player_id);
            void send_message(int from_id, int to_id, const rfcArgParser::DataPacket &data);
            void send_multicast(const rfcArgParser::DataPacket &data, const std::vector<int> &excludes);
            void send_multicast_excluded(const rfcArgParser::DataPacket &data, const std::vector<int> &excludes);
            void send_broadcast(const rfcArgParser::DataPacket &data);
            bool isRunning();

        private:
            void setupNetworkCallbacks();
            void handleNewConnection(int client_id);
            void handleMessage(int client_id, const std::string &data);
            Server::Network network_;
            std::unique_ptr<Command> command_processor_;
            std::vector<std::shared_ptr<RType::Game::Entity::Player>> clients_;
            std::vector<std::unique_ptr<RType::Game::Room>> rooms_;
            bool running_ = true;
    };
} // namespace Server

#endif // SERVER_HPP
