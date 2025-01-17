#include "Server.hpp"

namespace Server
{

    GameServer::GameServer(boost::asio::io_context &io_context, short port)
        : network_(io_context, port),
          command_processor_(std::make_unique<Command>(*this))
    {
        setupNetworkCallbacks();
        network_.start();
    }

    // Client management
    void GameServer::remove_client(int client_id)
    {
        clients_.erase(std::remove_if(
                           clients_.begin(),
                           clients_.end(),
                           [client_id](const auto &client) { return client->getId() == client_id; }),
                       clients_.end());

        auto packet = rfcArgParser::SerializePacket("disconnect", std::to_string(client_id));
        network_.broadcast(std::string(reinterpret_cast<char *>(&packet), sizeof(packet)));
    }

    const RType::Game::Entity::Player &GameServer::get_client(int client_id)
    {
        for (auto &player : clients_) {
            if (player->getId() == client_id)
                return *player;
        }
        throw std::runtime_error("Player not found.");
    }

    std::shared_ptr<RType::Game::Entity::Player> &GameServer::get_client_ptr(int client_id)
    {
        for (auto &player : clients_) {
            if (player->getId() == client_id)
                return player;
        }
        throw std::runtime_error("Player not found.");
    }

    bool GameServer::client_exist(int client_id)
    {
        return std::any_of(clients_.begin(), clients_.end(),
                           [client_id](const auto &player) { return player->getId() == client_id; });
    }

    // Room management
    int GameServer::add_room(const std::string &name)
    {
        auto room = RType::Game::Room::create(room_id++, name, command_processor_.get());
        rooms_.push_back(std::move(room));
        return room_id - 1;
    }

    int GameServer::add_room()
    {
        auto room = RType::Game::Room::create(room_id++, "ID=" + std::to_string(room_id), command_processor_.get());
        rooms_.push_back(std::move(room));
        return room_id - 1;
    }

    void GameServer::start_room(size_t index) const
    {
        if (index < rooms_.size()) {
            rooms_[index]->start();
        }
    }

    void GameServer::remove_room(int room_id)
    {
        rooms_.erase(std::remove_if(
                         rooms_.begin(),
                         rooms_.end(),
                         [room_id](const auto &room) { return room->getID() == room_id; }),
                     rooms_.end());
    }

    bool GameServer::add_player_to_room(int room_id, int player_id)
    {
        auto room_it = std::find_if(rooms_.begin(), rooms_.end(),
                                    [room_id](const auto &room) { return room->getID() == room_id; });

        if (room_it != rooms_.end()) {
            if ((*room_it)->addPlayer(get_client_ptr(player_id)) == false) {
                std::cerr << "Room is full." << std::endl;
                return false;
            }
            return true;
        } else {
            std::cerr << "Room not found." << std::endl;
            return false;
        }
    }

    void GameServer::remove_player_from_room(int room_id, int player_id)
    {
        auto room_it = std::find_if(rooms_.begin(), rooms_.end(),
                                    [room_id](const auto &room) { return room->getID() == room_id; });

        if (room_it != rooms_.end()) {
            (*room_it)->removePlayer(player_id);
        }
    }

    // Message sending
    void GameServer::send_message(int from_id, int to_id, const rfcArgParser::DataPacket &data)
    {
        network_.unicast(to_id, std::string(reinterpret_cast<const char *>(&data), sizeof(data)));
    }

    void GameServer::send_multicast(const rfcArgParser::DataPacket &data, const std::vector<int> &includes)
    {
        network_.multicast(
            std::string(reinterpret_cast<const char *>(&data), sizeof(data)),
            includes);
    }

    void GameServer::send_multicast_excluded(const rfcArgParser::DataPacket &data, const std::vector<int> &excludes)
    {
        network_.multicast_excluded(
            std::string(reinterpret_cast<const char *>(&data), sizeof(data)),
            excludes);
    }

    void GameServer::send_broadcast(const rfcArgParser::DataPacket &data)
    {
        network_.broadcast(std::string(reinterpret_cast<const char *>(&data), sizeof(data)));
    }

    void GameServer::setupNetworkCallbacks()
    {
        network_.setConnectCallback([this](int client_id) {
            handleNewConnection(client_id);
        });

        network_.setDisconnectCallback([this](int client_id) {
            remove_client(client_id);
        });

        network_.setMessageCallback([this](int client_id, const std::string &data) {
            handleMessage(client_id, data);
        });
    }

    void GameServer::handleNewConnection(int client_id)
    {
        Network::SocketPtr socket = network_.getClientSocket(client_id);
        auto new_client = std::make_shared<RType::Game::Entity::Player>(client_id, socket);
        clients_.push_back(new_client);
    }

    void GameServer::handleMessage(int client_id, const std::string &data)
    {
        rfcArgParser::DataPacket packet = rfcArgParser::DeserializePacket(
            data,
            sizeof(rfcArgParser::DataPacket));

        if (command_processor_) {
            command_processor_->process_command(client_id, packet);
        }
    }

    bool GameServer::isRunning()
    {
        return this->running_;
    }

    bool GameServer::isInRoom(int player_id)
    {
        std::shared_ptr<RType::Game::Entity::Player> player = get_client_ptr(player_id);
        return player->isInRoom();
    }

    std::unordered_map<std::string, std::string> GameServer::get_room_info(int room_id)
    {
        auto room_it = std::find_if(rooms_.begin(), rooms_.end(),
                                    [room_id](const auto &room) { return room->getID() == room_id; });

        if (room_it != rooms_.end()) {
            return (*room_it)->getRoomInfo();
        } else {
            return {};
        }
    }

    std::vector<std::unique_ptr<RType::Game::Room>> &GameServer::getRooms()
    {
        return rooms_;
    }

    RType::Game::Room *GameServer::getRoom(int room_id)
    {
        auto room_it = std::find_if(rooms_.begin(), rooms_.end(),
                                    [room_id](const auto &room) { return room->getID() == room_id; });

        if (room_it != rooms_.end()) {
            return room_it->get();
        } else {
            return nullptr;
        }
    }
} // namespace Server