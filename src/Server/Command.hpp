/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }
    >> Daniil STEPANOV - { daniil.stepanov@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef COMMAND_HPP
    #define COMMAND_HPP

    /*  ---- INCLUDES ---- */
    #include <RType.hpp>
    #include <Utils.hpp>
    #include <Server.hpp>
    #include <DataPacket.hpp>

#define UNUSED(x) (void)(x)

namespace Server
{
    class TCP;
    class Command
    {
        public:
            Command(TCP& tcp);
            void process_command(int client_id, Server::DataPacket packet);

        private:
            void start(int client_id, const std::string& args);
            void stop(int client_id, const std::string& args);
            void send(int client_id, const std::string& args);
            void broadcast(int client_id, const std::string& args);
            void position(int client_id, const std::string& args);
            std::unordered_map<std::string, std::function<void(int, const std::string&)>> commands_;
            TCP& tcp_;
    };
}

#endif //COMMAND_HPP
