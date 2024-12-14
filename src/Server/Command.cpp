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
    Command::Command(TCP &tcp) : tcp_(tcp)
    {
        /* COMMANDS READED*/
        commands_["position"]   = [this](const int client_id, const std::string &args) { position(client_id, args); };
        commands_["p_info"]     = [this](const int client_id, const std::string &args) { p_info(client_id, args); };
        commands_["shoot"]      = [this](const int client_id, const std::string &args) { shoot(client_id, args); };
        commands_["e_info"]     = [this](const int client_id, const std::string &args) { e_info(client_id, args); };

        /* COMMANDS TO SEND */
        send_["connect"]    = [this](const int receiver_id, const std::string &args, const std::string &command) { to_broadcast(receiver_id, args, command); };
        send_["disconnect"] = [this](const int receiver_id, const std::string &args, const std::string &command) { to_broadcast(receiver_id, args, command); };
        send_["p_position"] = [this](const int receiver_id, const std::string &args, const std::string &command) { to_broadcast(receiver_id, args, command); };
        send_["p_damage"]   = [this](const int receiver_id, const std::string &args, const std::string &command) { to_broadcast(receiver_id, args, command); };
        send_["p_shoot"]    = [this](const int receiver_id, const std::string &args, const std::string &command) { to_broadcast(receiver_id, args, command); };
        send_["p_death"]    = [this](const int receiver_id, const std::string &args, const std::string &command) { to_broadcast(receiver_id, args, command); };
        send_["e_position"] = [this](const int receiver_id, const std::string &args, const std::string &command) { to_broadcast(receiver_id, args, command); };
        send_["e_damage"]   = [this](const int receiver_id, const std::string &args, const std::string &command) { to_broadcast(receiver_id, args, command); };
        send_["e_shoot"]    = [this](const int receiver_id, const std::string &args, const std::string &command) { to_broadcast(receiver_id, args, command); };
        send_["e_death"]    = [this](const int receiver_id, const std::string &args, const std::string &command) { to_broadcast(receiver_id, args, command); };
        send_["color"]      = [this](const int receiver_id, const std::string &args, const std::string &command) { to_broadcast(receiver_id, args, command); };
        send_["wave"]       = [this](const int receiver_id, const std::string &args, const std::string &command) { to_broadcast(receiver_id, args, command); };
        send_["end"]        = [this](const int receiver_id, const std::string &args, const std::string &command) { to_broadcast(receiver_id, args, command); };

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
        Player player = tcp_.get_player(client_id);
        player.setPosX(x);
        player.setPosY(y);
    }

    void Command::p_info(int client_id, const std::string &args) {
        if (!args.empty()) {
            std::cerr << "Usage: p_info\n";
            return;
        }
        if (!RType::Utils::isNumber(args)) {
            std::cerr << "Invalid argument.\n";
            return;
        }
        int id = std::stoi(args);
        // Player player = Room.get_player(id); // TODO: make a get_player from the room class not the server
        // std::unordered_map<std::string, std::string> data = player.getPlayerInfo();
        // std::string data_str = rfcArgParser::CreateObject(data);
        // rfcArgParser::DataPacket packet = rfcArgParser::SerializePacket("p_info", data_str);
        // tcp_.send_message(SERVER_ID, client_id, packet);
    }

    void Command::shoot(int client_id, const std::string &args)
    {
        if (!args.empty()) {
            std::cerr << "Usage: shoot\n";
            return;
        }
        Player player = tcp_.get_player(client_id);
        std::unordered_map<std::string, std::string> data = rfcArgParser::ParseObject(args);
        if (!data.contains("x") || !data.contains("y")) {
            std::cerr << "Usage: shoot {\"x\": <x>, \"y\": <y>}\n";
            return;
        }
        if (!RType::Utils::isNumber(data["x"]) || !RType::Utils::isNumber(data["y"])) {
            std::cerr << "Invalid x or y value.\n";
            return;
        }

        player.shoot(std::stoi(data["x"]), std::stoi(data["y"]));
    }

    void e_info(int client_id, const std::string &args)
    {
        if (!args.empty()) {
            std::cerr << "Usage: e_info\n";
            return;
        }
        if (!RType::Utils::isNumber(args)) {
            std::cerr << "Invalid argument.\n";
            return;
        }
        int id = std::stoi(args);
        // Monster monster = Room.getEnemyInfo(id); // TODO: depends on room class
        // std::unordered_map<std::string, std::string> data = monster.getEnemyInfo();
        // std::string data_str = rfcArgParser::CreateObject(data);
        // rfcArgParser::DataPacket packet = rfcArgParser::SerializePacket("e_info", data_str);
        // tcp_.send_message(SERVER_ID, client_id, packet);
    }

    void Command::to_send(const int receiver_id, const std::string &args, const std::string &command)
    {
        std::string data = rfcArgParser::CreateObject(rfcArgParser::ParseObject(args));
        rfcArgParser::DataPacket packet = rfcArgParser::SerializePacket(command, data);
        tcp_.send_message(SERVER_ID, receiver_id, packet);
    }

    void Command::to_broadcast(const int receiver_id, const std::string &args, const std::string &command)
    {
        std::string data = rfcArgParser::CreateObject(rfcArgParser::ParseObject(args));
        rfcArgParser::DataPacket packet = rfcArgParser::SerializePacket(command, data);
        tcp_.send_broadcast(packet);
    }

} // namespace Server
