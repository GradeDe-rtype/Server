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
        commands_["create"] = [this](const int client_id, const std::string &args) { create(client_id, args); };
        commands_["join"] = [this](const int client_id, const std::string &args) { join(client_id, args); };
        commands_["list"] = [this](const int client_id, const std::string &args) { list(client_id, args); };
        commands_["r_info"] = [this](const int client_id, const std::string &args) { r_info(client_id, args); };
        // commands_["e_info"] = [this](const int client_id, const std::string &args) { e_info(client_id, args); };

        /* COMMANDS TO SEND */
        send_["connect"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["disconnect"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["p_position"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["p_damage"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["p_death"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["e_position"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["e_damage"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["e_death"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["s_position"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["s_death"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
        send_["shoot"] = [this](const int receiver_id, const std::string &command, const std::string &args) { to_broadcast(receiver_id, command, args); };
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
            it->second(receiver_id, command, args);
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
        if (server_.isInRoom(client_id) == false) {
            std::cerr << client_id << " is not in room.\n";
            return;
        }
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
        const RType::Game::Entity::Player &player = server_.get_client(client_id);
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
        if (server_.isInRoom(client_id) == false) {
            std::cerr << client_id << " is not in room.\n";
            return;
        }
        if (!args.empty()) {
            std::cerr << "Usage: p_info\n";
            return;
        }
        if (!RType::Utils::isNumber(args)) {
            std::cerr << "Invalid argument.\n";
            return;
        }
        int id = std::stoi(args);
        const RType::Game::Entity::Player &player = server_.get_client(client_id); // TODO: make a get_player from the room class not the server
        std::unordered_map<std::string, std::string> data = player.getPlayerInfo();
        std::string data_str = rfcArgParser::CreateObject(data);
        rfcArgParser::DataPacket packet = rfcArgParser::SerializePacket("p_info", data_str);
        server_.send_message(SERVER_ID, client_id, packet);
    }

    void Command::shoot(int client_id, const std::string &args)
    {
        if (server_.isInRoom(client_id) == false) {
            std::cerr << client_id << " is not in room.\n";
            return;
        }
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
        if (player->getWeapon() == RType::Game::Entity::Player::SHOTGUN_SHOOT) {
            for (int i = 0; i < 3; i++) {
                int offset = (i == 0) ? -10 : (i == 2) ? 10 : 0;
                player->shoot(std::stoi(data["x"]), std::stoi(data["y"]) + offset);

                std::string shoot = rfcArgParser::CreateObject(player->getShoots().back()->getShootInfo());
                rfcArgParser::DataPacket packet = rfcArgParser::SerializePacket("shoot", shoot);

                server_.send_broadcast(packet);
            }

        }
        player->shoot(std::stoi(data["x"]), std::stoi(data["y"]));
        std::string shoot = rfcArgParser::CreateObject(player->getShoots().back()->getShootInfo());
        rfcArgParser::DataPacket packet = rfcArgParser::SerializePacket("shoot", shoot);
        server_.send_broadcast(packet);
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
        if (server_.isInRoom(client_id) == false) {
            std::cerr << client_id << " is not in room.\n";
            return;
        }
        UNUSED(args);
        std::shared_ptr<RType::Game::Entity::Player> player = server_.get_client_ptr(client_id);
        player->setHaveJoined(true);
    }

    void Command::create(int client_id, const std::string &args)
    {
        if (server_.isInMenu(client_id) == false) {
            std::cerr << client_id << " is not in menu.\n";
            return;
        }
        UNUSED(args);
        std::cout << "Creating room for " << client_id << std::endl;
        server_.add_room(std::to_string(client_id) + "'s-room");
        server_.send_message(SERVER_ID, client_id, rfcArgParser::SerializePacket("create", std::to_string(client_id)));
    }

    void Command::join(int client_id, const std::string &args)
    {
        if (server_.isInMenu(client_id) == false) {
            std::cerr << client_id << " is not in menu.\n";
            return;
        }
        if (!RType::Utils::isNumber(args)) {
            std::cerr << "Invalid argument.\n";
            return;
        }
        int room_id = std::stoi(args);
        server_.add_player_to_room(room_id, client_id);
        std::unordered_map<std::string, std::string> data = server_.get_room_info(room_id);
        if (data.empty()) {
            std::cerr << "Room not found.\n";
            return;
        }
        server_.get_client_ptr(client_id)->setInRoom(true);
        std::vector<int> includes;
        for (const auto &[_, player] : server_.getRoom(room_id)->getPlayers())
            includes.push_back(player->getId());
        unsigned long color_size = includes.size();
        server_.send_message(SERVER_ID, client_id, rfcArgParser::SerializePacket("join", rfcArgParser::CreateObject(data)));
        server_.get_client_ptr(client_id)->setColor(RType::Game::Colors::get().getColor(color_size));
        auto packet = rfcArgParser::SerializePacket(
            "connect",
            rfcArgParser::CreateObject(server_.get_client_ptr(client_id)->getPlayerSmallInfo()));
        server_.send_multicast(packet, includes);

        packet = rfcArgParser::SerializePacket(
            "connect_you",
            rfcArgParser::CreateObject(server_.get_client_ptr(client_id)->getPlayerSmallInfo()));
        server_.send_message(SERVER_ID, client_id, packet);

        for (const auto &[_, player] : server_.getRoom(room_id)->getPlayers()) {
            if (player->getId() != client_id) {
                packet = rfcArgParser::SerializePacket(
                    "connect",
                    rfcArgParser::CreateObject(player->getPlayerSmallInfo()));
                server_.send_message(SERVER_ID, client_id, packet);
            }
        }
        RType::Game::Room *room = server_.getRoom(room_id);
        if (room->isRunning() == false)
            room->start();
    }

    void Command::list(int client_id, const std::string &args)
    {
        if (server_.isInMenu(client_id) == false) {
            std::cerr << client_id << " is not in menu.\n";
            return;
        }
        std::vector<std::unique_ptr<RType::Game::Room>> &rooms = server_.getRooms();
        std::vector<int> room_ids;
        for (const auto &room : rooms)
            room_ids.push_back(room->getID());
        std::string data;
        data += "[";
        for (const auto &id : room_ids) {
            if (id != room_ids.front())
                data += ";";
            if (id != room_ids.back())
                data += std::to_string(id);
            else
                data += std::to_string(id);
        }
        data += "]";
        std::cout << "List of rooms: " << data << std::endl;
        rfcArgParser::DataPacket packet = rfcArgParser::SerializePacket("list", data);
        server_.send_message(SERVER_ID, client_id, packet);
    }

    void Command::r_info(int client_id, const std::string &args)
    {
        if (server_.isInMenu(client_id) == false) {
            std::cerr << client_id << " is not in room.\n";
            return;
        }
        int id = std::stoi(args);
        std::shared_ptr<RType::Game::Entity::Player> player = server_.get_client_ptr(client_id);
        rfcArgParser::DataPacket packet =
            rfcArgParser::SerializePacket("r_info", rfcArgParser::CreateObject(server_.get_room_info(id)));
        server_.send_message(SERVER_ID, client_id, packet);
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
