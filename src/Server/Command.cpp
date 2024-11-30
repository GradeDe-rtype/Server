/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }
    >> Daniil STEPANOV - { daniil.stepanov@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include <Command.hpp>

namespace Server {

    Command::Command(TCP& tcp) : tcp_(tcp) {
        commands_["start"] = [this](int client_id, const std::string& args) { start(client_id, args); };
        commands_["send"] = [this](int client_id, const std::string& args) { send(client_id, args); };
        commands_["stop"] = [this](int client_id, const std::string& args) { stop(client_id, args); };
    }

    void Command::process_command(int client_id, const std::string& message) {
        const auto space_pos = message.find(' ');
        std::string command = RType::Utils::normalize(message.substr(0, space_pos));
        std::string args = RType::Utils::trim((space_pos != std::string::npos) ? message.substr(space_pos + 1) : "");

        auto it = commands_.find(command);
        if (it != commands_.end()) {
            it->second(client_id, args);
        } else {
            std::cerr << "Unknown command: |" << command << "|\n";
            std::cerr << "Available commands are:\n";
            for (const auto& [key, _] : commands_) {
                std::cerr << "- |" << key << "|\n";
            }
        }
    }

    void Command::start(const int client_id, const std::string& args) {
        std::cout << "Client " << client_id << " started.\n";
    }

    void Command::stop(const int client_id, const std::string& args) {
        std::cout << "Client " << client_id << " stopped.\n";
    }

    void Command::send(const int client_id, const std::string& args) {
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
        if (tcp_.get_clients().find(id) == tcp_.get_clients().end()) {
            std::cerr << "Client " << id << " not found.\n";
            return;
        }
        std::string msg;
        for (size_t i = 1; i < words.size(); ++i) {
            if (i > 1)
                msg += " ";
            msg += words[i];
        }

        const std::string message = msg + "\n";
        tcp_.send_message(client_id, id, message);
    }
};