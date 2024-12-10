/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }
    >> Daniil STEPANOV - { daniil.stepanov@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include <Command.hpp>
#include <Player.hpp>

namespace Server
{
    Command::Command(TCP &tcp) : tcp_(tcp)
    {
        commands_["start"] = [this](const int client_id, const std::string &args) { start(client_id, args); };
        commands_["send"] = [this](const int client_id, const std::string &args) { send(client_id, args); };
        commands_["stop"] = [this](const int client_id, const std::string &args) { stop(client_id, args); };
        commands_["broadcast"] = [this](const int client_id, const std::string &args) { broadcast(client_id, args); };
        commands_["position"] = [this](const int client_id, const std::string &args) { position(client_id, args); };
    }

    void Command::process_command(const int client_id, Server::DataPacket packet)
    {
        std::string args_str(packet.args);
        const std::string command = packet.command;
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

    void Command::start(const int client_id, const std::string &args)
    {
        std::cout << "Client " << client_id << " started.\n";
    }

    void Command::stop(const int client_id, const std::string &args)
    {
        std::cout << "Client " << client_id << " stopped.\n";
        tcp_.setRunning(false);
    }

    void Command::send(const int client_id, const std::string &args)
    {
        if (args.empty()) {
            std::cerr << "No message to send.\n";
            return;
        }
        std::vector<std::string> words;
        std::istringstream iss(args);
        std::string word;
        while (iss >> word) {
            words.push_back(word);
        }
        if (words.size() < 2) {
            std::cerr << "Usage: send <client_id> <message>\n";
            return;
        }
        if (!RType::Utils::isNumber(words[0])) {
            std::cerr << "Invalid client id: |" << words[0] << "|\n";
            return;
        }
        const int id = std::stoi(words[0]);
        if (!tcp_.player_exists(id)) {
            std::cerr << "Client " << id << " does not exist.\n";
            return;
        }
        std::string message;
        for (size_t i = 1; i < words.size(); ++i) {
            if (i > 1)
                message += " ";
            message += words[i];
        }
        Server::DataPacket data = RType::Utils::createDataPacket("message", message);
        tcp_.send_message(client_id, id, data);
    }

    void Command::broadcast(const int client_id, const std::string &args)
    {
        UNUSED(client_id);
        std::vector<std::string> words;
        std::istringstream iss(args);
        std::string word;
        std::string message;
        while (iss >> word) {
            words.push_back(word);
        }
        std::vector<int> excluded_clients;
        for (const auto &elem : words) {
            if (RType::Utils::isNumber(elem)) {
                excluded_clients.push_back(std::stoi(elem));
            } else {
                if (word == "broadcast") {
                    continue;
                }
                if (!message.empty())
                    message += " ";
                message += elem;
            }
        }
        const Server::DataPacket data = RType::Utils::createDataPacket("broadcast", message);
        tcp_.send_broadcast(data, excluded_clients);
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

        std::string temporary = rfcArgParser::CreateObject(obj);
        temporary = std::to_string(client_id) + " " + temporary + "\n";
        Server::DataPacket data = RType::Utils::createDataPacket("position", temporary);
        tcp_.send_broadcast(data, {client_id});
    }
} // namespace Server
