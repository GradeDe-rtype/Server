/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }
    >> Daniil STEPANOV - { daniil.stepanov@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

/*  ---- INCLUDES ---- */
#include "Server.hpp"
#include "Player.hpp"

namespace Server
{
    TCP::TCP(boost::asio::io_context &io_context, const short port) : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
                                                                      command_processor(new Command(*this))
    {
        start_accept();
        add_room(0, "First Room");
        start_room(0);
    }

    TCP::~TCP() { delete command_processor; }

    void TCP::remove_client(int client_id)
    {
        clients_.erase(std::remove_if(clients_.begin(), clients_.end(),
                                      [client_id](const std::shared_ptr<RType::Game::Entity::Player> &client) { return client->getId() == client_id; }),
                      clients_.end());
        send_broadcast(rfcArgParser::SerializePacket("disconnect", std::to_string(client_id)));
    }

    RType::Game::Entity::Player &TCP::get_client(const int client_id)
    {
        for (auto &player : clients_) {
            if (player->getId() == client_id)
                return *player;
        }
        throw std::runtime_error("Player not found.");
    }

    std::shared_ptr<RType::Game::Entity::Player> &TCP::get_client_ptr(const int client_id)
    {
        for (auto &player : clients_) {
            if (player->getId() == client_id)
                return player;
        }
        throw std::runtime_error("Player not found.");
    }

    bool TCP::client_exist(const int client_id)
    {
        for (const auto &player : clients_) {
            if (player->getId() == client_id)
                return true;
        }
        return false;
    }

    void TCP::add_client(std::shared_ptr<RType::Game::Entity::Player> client)
    {
        clients_.push_back(client);
    }

    void TCP::add_room(int id, const std::string& name)
    {
        auto room = RType::Game::Room::create(id, name, command_processor);
        rooms_.push_back(std::move(room));
    }

    void TCP::start_room(size_t index)
    {
        if (index < rooms_.size())
        {
            rooms_[index]->start();
        }
    }

    void TCP::remove_room(const int room_id)
    {
        rooms_.erase(std::remove_if(rooms_.begin(), rooms_.end(),
                                    [room_id](const std::unique_ptr<RType::Game::Room> &room) { return room->getID() == room_id; }),
                     rooms_.end());
    }

    void TCP::add_player_to_room(const int room_id, const int player_id)
    {
        for (auto &room : rooms_) {
            if (room->getID() == room_id) {
                room->addPlayer(get_client_ptr(player_id));
                return;
            }
        }
    }

    void TCP::remove_player_from_room(const int room_id, const int player_id)
    {
        for (auto &room : rooms_) {
            if (room->getID() == room_id) {
                room->removePlayer(player_id);
                return;
            }
        }
    }

    void TCP::start_accept()
    {
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(acceptor_.get_executor());
        acceptor_.async_accept(
            *socket,
            [this, socket](const boost::system::error_code &error) {
                if (!error) {
                    RType::Game::Entity::Player new_client(next_client_id_++, socket);
                    std::unordered_map<std::string, std::string> data;
                    rfcArgParser::DataPacket packet{};
                    add_client(std::make_shared<RType::Game::Entity::Player>(new_client));
                    std::cout << "Client " << new_client.getId() << " connected.\n";
                    start_read(new_client);
                    packet = rfcArgParser::SerializePacket("connect", rfcArgParser::CreateObject(new_client.getPlayerSmallInfo()));
                    send_multicast(packet, {new_client.getId()});
                    new_client.setColor("#FFFFFF");
                    packet = rfcArgParser::SerializePacket("connect_you", rfcArgParser::CreateObject(new_client.getPlayerSmallInfo()));
                    send_message(-1, new_client.getId(), packet);
                    for (const auto &client : clients_) {
                        if (client->getId() == new_client.getId())
                            continue;
                        packet = rfcArgParser::SerializePacket("connect", rfcArgParser::CreateObject(client->getPlayerSmallInfo()));
                        send_message(-1, new_client.getId(), packet);
                    }
                    add_player_to_room(0, new_client.getId()); // TODO: in the future add a room selection
                } else {
                    std::cerr << "Accept error: " << error.message() << std::endl;
                }
                start_accept(); // Accept the next connection
            });
    }

    void TCP::start_read(RType::Game::Entity::Player client)
    {
        const auto socket = client.getSocket();
        const int client_id = client.getId();
        if (!socket) {
            std::cerr << "Socket for client " << client_id << " is null.\n";
            return;
        }
        auto buffer = std::make_shared<std::array<char, sizeof(rfcArgParser::DataPacket)>>();
        boost::asio::async_read(*socket, boost::asio::buffer(*buffer),
                                [this, client_id, buffer, client](const boost::system::error_code &error, const std::size_t length) {
                                    if (!error) {
                                        std::string data(buffer->begin(), buffer->begin() + length);
                                        rfcArgParser::DataPacket packet = rfcArgParser::DeserializePacket(data, length);

                                        std::cout << "Client " << client_id << " sent a packet with command: " << packet.command << "\n";
                                        std::cout << "Arguments: " << packet.args << "\n";
                                        if (command_processor) {
                                            command_processor->process_command(client_id, packet);
                                        } else {
                                            std::cerr << "Command processor is not initialized.\n";
                                        }
                                        start_read(client);
                                    } else {
                                        std::cerr << "Client " << client_id << " disconnected: " << error.message() << "\n";
                                        remove_client(client_id);
                                    }
                                });
    }

    void TCP::send_message(int client_id, int receiver_id, rfcArgParser::DataPacket data)
    {
        for (const auto &client : clients_) {
            const int id = client->getId();
            const auto socket = client->getSocket();
            if (id == receiver_id) {
                boost::asio::async_write(
                    *socket, boost::asio::buffer(&data, sizeof(data)),
                    [client_id, receiver_id](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                        if (!ec) {
                            std::cout << "Message sent from client " << client_id << " to receiver " << receiver_id
                                      << ", bytes: " << bytes_transferred << std::endl;
                        } else {
                            std::cerr << "Error sending message: " << ec.message() << std::endl;
                        }
                    });
                return;
            }
        }
    }

    void TCP::send_multicast(rfcArgParser::DataPacket data, const std::vector<int> &excludes)
    {
        for (const auto &client : clients_) {
            const int id = client->getId();
            if (std::find(excludes.begin(), excludes.end(), id) != excludes.end()) continue;
            const auto socket = client->getSocket();
            std::cout << "Sending multicast to client " << id << std::endl;
            boost::asio::async_write(*socket, boost::asio::buffer(&data, sizeof(data)),
                                     [](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                                         if (!ec) {
                                             std::cout << "Broadcast sent, bytes: " << bytes_transferred << std::endl;
                                         } else {
                                             std::cerr << "Error sending broadcast: " << ec.message() << std::endl;
                                         }
                                     });
        }
    }

    void TCP::send_multicast_excluded(rfcArgParser::DataPacket data, const std::vector<int> &excluded_clients) {
        for (const auto &client : clients_) {
            const int id = client->getId();
            const auto socket = client->getSocket();
            if (std::find(excluded_clients.begin(), excluded_clients.end(), id) != excluded_clients.end())
                continue;
            boost::asio::async_write(*socket, boost::asio::buffer(&data, sizeof(data)),
                                     [](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                                         if (!ec) {
                                             std::cout << "Broadcast sent, bytes: " << bytes_transferred << std::endl;
                                         } else {
                                             std::cerr << "Error sending broadcast: " << ec.message() << std::endl;
                                         }
                                     });
            return;
        }
    }

    void TCP::send_broadcast(rfcArgParser::DataPacket data)
    {
        for (const auto &client : clients_) {
            const int id = client->getId();
            const auto socket = client->getSocket();
            boost::asio::async_write(*socket, boost::asio::buffer(&data, sizeof(data)),
                                     [](const boost::system::error_code &ec, std::size_t bytes_transferred) {
                                         if (!ec) {
                                             std::cout << "Broadcast sent, bytes: " << bytes_transferred << std::endl;
                                         } else {
                                             std::cerr << "Error sending broadcast: " << ec.message() << std::endl;
                                         }
                                     });
        }
    }
} // namespace Server
