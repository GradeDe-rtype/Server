/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }
    >> Daniil STEPANOV - { daniil.stepanov@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include "Command.hpp"
#include "Player.hpp"

namespace Server
{
    Command::Command(Server::GameServer &server) : server_(server)
    {
        /* COMMANDS READED*/
        commands_["position"] = [this](const int client_id, const std::string &args) { position(client_id, args); };
        commands_["p_info"] = [this](const int client_id, const std::string &args) { p_info(client_id, args); };
        commands_["shoot"] = [this](const int client_id, const std::string &args) { shoot(client_id, args); };
        commands_["ready"] = [this](const int client_id, const std::string &args) { ready(client_id, args); };
        // commands_["e_info"] = [this](const int client_id, const std::string &args) { e_info(client_id, args); };

        /* COMMANDS TO SEND */
        send_["connect"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["disconnect"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["p_position"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["p_damage"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["p_shoot"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["p_death"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["e_position"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["e_damage"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["e_shoot"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["e_death"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["color"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["wave"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["end"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["enemy"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
    }

    void Command::process_command(const int client_id, rfcArgParser::DataPacket packet)
    {
        const std::string command = RType::Utils::trim(packet.command);
        const std::string args = RType::Utils::trim(packet.args);

        if (const auto it = commands_.find(command); it != commands_.end()) {
            it->second(client_id, args);
        } else {
            std::cerr << "Unknown command: |" << command << "|\n";
            std::cerr << "Available commands are:\n";
            for (const auto &[key, _] : commands_) {
                std::cerr << "- |" << key << "|\n";
            }
        }
    }

    void Command::send(const int receiver_id, const std::string &command, const std::string &args)
    {
        if (const auto it = send_.find(command); it != send_.end()) {
            it->second(receiver_id, args, command);
        } else {
            std::cerr << "Unknown command: |" << command << "|\n";
            std::cerr << "Available commands are:\n";
            for (const auto &[key, _] : send_) {
                std::cerr << "- |" << key << "|\n";
            }
        }
    }

    void Command::position(int client_id, const std::string &args)
    {
        std::unordered_map<std::string, std::string> obj = rfcArgParser::ParseObject(args);
        if (!obj.contains("x") || !obj.contains("y")) {
            std::cerr << "Usage: position {\"x\": <x>, \"y\": <y>}\n";
            return;
        }
        if (!RType::Utils::isNumber(obj["x"]) || !RType::Utils::isNumber(obj["y"])) {
            std::cerr << "Invalid x or y value.\n";
            return;
        }
        const int x = std::stoi(obj["x"]);
        const int y = std::stoi(obj["y"]);
        RType::Game::Entity::Player player = server_.get_client(client_id);
        std::shared_ptr<RType::Game::Entity::Player> p = server_.get_client_ptr(client_id);
        p->setPosition({x, y});

        std::unordered_map<std::string, std::string> data;
        data["x"] = std::to_string(player.getPosX());
        data["y"] = std::to_string(player.getPosY());
        std::string data_str = rfcArgParser::CreateObject(data);
        data_str = std::to_string(client_id) + " " + data_str;
        rfcArgParser::DataPacket packet = rfcArgParser::SerializePacket("p_position", data_str);
        server_.send_broadcast(packet);
    }

    void Command::p_info(int client_id, const std::string &args)
    {
        if (!args.empty()) {
            std::cerr << "Usage: p_info\n";
            return;
        }
        if (!RType::Utils::isNumber(args)) {
            std::cerr << "Invalid argument.\n";
            return;
        }
        int id = std::stoi(args);
        RType::Game::Entity::Player player = server_.get_client(client_id); // TODO: make a get_player from the room class not the server
        std::unordered_map<std::string, std::string> data = player.getPlayerInfo();
        std::string data_str = rfcArgParser::CreateObject(data);
        rfcArgParser::DataPacket packet = rfcArgParser::SerializePacket("p_info", data_str);
        server_.send_message(SERVER_ID, client_id, packet);
    }

    void Command::shoot(int client_id, const std::string &args)
    {
        std::shared_ptr<RType::Game::Entity::Player> player = server_.get_client_ptr(client_id);
        std::unordered_map<std::string, std::string> data = rfcArgParser::ParseObject(args);
        if (!data.contains("x") || !data.contains("y")) {
            std::cerr << "Usage: shoot {\"x\": <x>, \"y\": <y>}\n";
            return;
        }
        if (!RType::Utils::isNumber(data["x"]) || !RType::Utils::isNumber(data["y"])) {
            std::cerr << "Invalid x or y value.\n";
            return;
        }

        player->shoot(std::stoi(data["x"]), std::stoi(data["y"]));
        std::unordered_map<std::string, std::string> shoot_data;
        shoot_data["x"] = std::to_string(player->getShoots().back()->getPosX());
        shoot_data["y"] = std::to_string(player->getShoots().back()->getPosY());
        std::string shoot_data_str = rfcArgParser::CreateObject(shoot_data);
        rfcArgParser::DataPacket packet = rfcArgParser::SerializePacket("p_shoot", shoot_data_str);
        server_.send_multicast_excluded(packet, {client_id});
    }

    // void e_info(int client_id, const std::string &args)
    // {
    //     if (!args.empty()) {
    //         std::cerr << "Usage: e_info\n";
    //         return;
    //     }
    //     if (!RType::Utils::isNumber(args)) {
    //         std::cerr << "Invalid argument.\n";
    //         return;
    //     }
    //     int id = std::stoi(args);
    //     // Monster monster = Room.getEnemyInfo(id); // TODO: depends on room class
    //     // std::unordered_map<std::string, std::string> data = monster.getEnemyInfo();
    //     // std::string data_str = rfcArgParser::CreateObject(data);
    //     // rfcArgParser::DataPacket packet = rfcArgParser::SerializePacket("e_info", data_str);
    //     // server_.send_message(SERVER_ID, client_id, packet);
    // }

    void Command::ready(int client_id, const std::string &args)
    {
        UNUSED(args);
        std::shared_ptr<RType::Game::Entity::Player> player = server_.get_client_ptr(client_id);
        player->setHaveJoined(true);
    }

    void Command::to_send(const int receiver_id, const std::string &command, const std::string &args)
    {
        rfcArgParser::DataPacket packet = rfcArgParser::SerializePacket(command, args);
        server_.send_message(receiver_id, receiver_id, packet);
    }

    void Command::to_broadcast(const int receiver_id, const std::string &command, const std::string &args)
    {
        rfcArgParser::DataPacket packet = rfcArgParser::SerializePacket(command, args);
        server_.send_broadcast(packet);
    }

} // namespace Server
