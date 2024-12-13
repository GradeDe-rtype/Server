/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }
    >> Daniil STEPANOV - { daniil.stepanov@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

/*  ---- INCLUDES ---- */
#include <Player.hpp>
#include <Server.hpp>

namespace Server
{
    TCP::TCP(boost::asio::io_context &io_context, const short port) : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
                                                                    command_processor(new Command(*this))
    {
        start_accept();
    }

    TCP::~TCP() { delete command_processor; }

    void TCP::remove_player(int client_id)
    {
        players_.erase(std::remove_if(players_.begin(), players_.end(),
                                    [client_id](const Player &player) { return player.getId() == client_id; }),
                    players_.end());
    }

    Player &TCP::get_player(const int client_id)
    {
        for (auto &player : players_) {
            if (player.getId() == client_id)
                return player;
        }
        throw std::runtime_error("Player not found.");
    }

    bool TCP::player_exists(const int client_id)
    {
        for (const auto &player : players_) {
            if (player.getId() == client_id)
                return true;
        }
        return false;
    }

    void TCP::start_accept()
    {
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(acceptor_.get_executor());
        acceptor_.async_accept(
            *socket,
            [this, socket](const boost::system::error_code &error) {
                if (!error) {
                    Player new_player(next_client_id_++, socket);
                    std::unordered_map<std::string, std::string> data;
                    rfcArgParser::DataPacket packet{};

                    players_.push_back(new_player);
                    std::cout << "Client " << new_player.getId() << " connected.\n";
                    std::string welcome_message = "Welcome, Client " + std::to_string(new_player.getId()) + "!\n";
                    boost::asio::async_write(*socket, boost::asio::buffer(welcome_message),
                                            [](const boost::system::error_code &, std::size_t) {});
                    start_read(new_player);
                    data["player_id"] = std::to_string(new_player.getId());
                    data["color"] = "#FF0000";
                    packet = rfcArgParser::SerializePacket("connect", rfcArgParser::CreateObject(data));
                    send_broadcast(packet, {new_player.getId()});
                    for (const auto &player : players_) {
                        if (player.getId() == new_player.getId())
                            continue;
                        std::unordered_map<std::string, std::string> data;
                        data["player_id"] = std::to_string(player.getId());
                        data["color"] = "#FF0000";
                        packet = rfcArgParser::SerializePacket("connect", rfcArgParser::CreateObject(data));
                        send_message(-1, new_player.getId(), packet);
                    }
                } else {
                    std::cerr << "Accept error: " << error.message() << std::endl;
                }
                start_accept(); // Accept the next connection
            });
    }

    void TCP::start_read(Player player)
    {
        const auto socket = player.getSocket();
        const int client_id = player.getId();
        if (!socket) {
            std::cerr << "Socket for client " << client_id << " is null.\n";
            return;
        }
        auto buffer = std::make_shared<std::array<char, sizeof(rfcArgParser::DataPacket)>>();
        boost::asio::async_read(*socket, boost::asio::buffer(*buffer),
                                [this, client_id, buffer, player](const boost::system::error_code &error, const std::size_t length) {
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
                                        start_read(player);
                                    } else {
                                        std::cerr << "Client " << client_id << " disconnected: " << error.message() << "\n";
                                        remove_player(client_id);
                                    }
                                });
    }

    void TCP::send_message(int client_id, int receiver_id, rfcArgParser::DataPacket data)
    {
        for (const auto &player : players_) {
            const int id = player.getId();
            const auto socket = player.getSocket();
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

    void TCP::send_broadcast(rfcArgParser::DataPacket data, const std::vector<int> &excluded_clients)
    {
        for (const auto &player : players_) {
            const int id = player.getId();
            const auto socket = player.getSocket();
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
} // namespace Server
