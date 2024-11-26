//
// Created by dan13615 on 11/26/24.
//

#include "../includes/command.hpp"

namespace Server {

    Command::Command() {
        commands_["start"] = [this](int client_id, const std::string& args) { start(client_id, args); };
        commands_["stop"] = [this](int client_id, const std::string& args) { stop(client_id, args); };
    }

    void Command::process_command(int client_id, const std::string& message) {
        const auto space_pos = message.find(' ');
        std::string command = normalize(message.substr(0, space_pos));
        std::string args = trim((space_pos != std::string::npos) ? message.substr(space_pos + 1) : "");

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
};