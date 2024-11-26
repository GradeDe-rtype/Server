//
// Created by dan13615 on 11/26/24.
//

#ifndef COMMAND_HPP
    #define COMMAND_HPP

#include <string>
#include <unordered_map>
#include <functional>
#include "utils.hpp"
#include <iostream>

namespace Server {
    class Command {
    public:
        Command();
        void process_command(int client_id, const std::string& message);

    private:
        void start(int client_id, const std::string& args);
        void stop(int client_id, const std::string& args);

        std::unordered_map<std::string, std::function<void(int, const std::string&)>> commands_;
    };
}

#endif //COMMAND_HPP
