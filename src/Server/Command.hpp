/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }
    >> Daniil STEPANOV - { daniil.stepanov@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef COMMAND_HPP
#define COMMAND_HPP

#define SERVER_ID -1

/*  ---- INCLUDES ---- */
#include "RType.hpp"
#include "Server.hpp"
#include "Utils.hpp"
#include "rfcArgParser.hpp"

#define UNUSED(x) (void)(x)

namespace Server
{
    class GameServer;
    class Command
    {
        public:
            Command(Server::GameServer &server);
            void process_command(int client_id, rfcArgParser::DataPacket packet);
            void send(int receiver_id, const std::string &command, const std::string &args);
            void to_send(int receiver_id, const std::string &command, const std::string &args);
            void to_broadcast(int receiver_id, const std::string &command, const std::string &args);

        private:
            void position(int client_id, const std::string &args);
            void p_info(int client_id, const std::string &args);
            void shoot(int client_id, const std::string &args);
            void ready(int client_id, const std::string &args);
            void e_info(int client_id, const std::string &args);
            std::unordered_map<std::string, std::function<void(int, const std::string &)>> commands_;
            std::unordered_map<std::string, std::function<void(int, const std::string &, const std::string &)>> send_;
            GameServer &server_;
    };
} // namespace Server

#endif // COMMAND_HPP
