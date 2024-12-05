/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }
    >> Daniil STEPANOV - { daniil.stepanov@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

/*  ---- INCLUDES ---- */
#include <Server.hpp>
#include <Player.hpp>

namespace Server {
    TCP::TCP(boost::asio::io_context& io_context, const short port)
    : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
    command_processor(new Command(*this))
    {
        start_accept();
    }

    TCP::~TCP() {
        delete command_processor;
    }

    void TCP::remove_player(int client_id) {
        players_.erase(std::remove_if(players_.begin(), players_.end(),
            [client_id](const Player& player) {
                return player.getId() == client_id;
            }), players_.end());
    }

    Player& TCP::get_player(int client_id) {
        for (auto& player : players_) {
            if (player.getId() == client_id)
                return player;
        }
        throw std::runtime_error("Player not found.");
    }

    bool TCP::player_exists(int client_id) {
        for (const auto& player : players_) {
            if (player.getId() == client_id)
                return true;
        }
        return false;

    }

    void TCP::start_accept() {
        auto socket = std::make_shared<boost::asio::ip::tcp::socket>(acceptor_.get_executor());
        acceptor_.async_accept(*socket, [this, socket](const boost::system::error_code& error) {
            if (!error) {
                Player player(next_client_id_++, socket);
                players_.push_back(player);
                std::cout << "Client " << player.getId() << " connected.\n";
                std::string welcome_message = "Welcome, Client " + std::to_string(player.getId()) + "!\n";
                boost::asio::async_write(*socket, boost::asio::buffer(welcome_message),
                    [](const boost::system::error_code&, std::size_t) {});
                start_read(player);
                std::unordered_map<std::string, std::string> data;
                data["player_id"] = std::to_string(player.getId());
                data["color"] = "#FF0000";
                send_broadcast("connect " + rfcArgParser::CreateObject(data), {player.getId()});
            } else {
                std::cerr << "Accept error: " << error.message() << std::endl;
            }
            start_accept(); // Accept the next connection
        });
    }

    void TCP::start_read(Player player) {
        const auto socket = player.getSocket();
        const int client_id = player.getId();
        auto buffer = std::make_shared<std::string>(1024, '\0');

        boost::asio::async_read_until(*socket, boost::asio::dynamic_buffer(*buffer), '\n',
    [this, client_id, buffer, player](const boost::system::error_code& error, const std::size_t length) {
        if (!error) {
            std::string raw_message = buffer->substr(0, length);
            buffer->erase(0, length);
            std::string sanitized_message = RType::Utils::trim(raw_message);
            std::cout << "Client " << client_id << " says: " << sanitized_message << "\n";
            command_processor->process_command(client_id, sanitized_message);
            start_read(player); // Continue reading
        } else {
            std::cerr << "Client " << client_id << " disconnected.\n";
            remove_player(client_id);
            send_broadcast("disconnect " + std::to_string(client_id), {client_id});
        }
    });
    }

    void TCP::send_message(int client_id, int receiver_id, const std::string& message) {

        for (const auto & player : players_) {
            const int id = player.getId();
            const auto socket = player.getSocket();
            if (id == receiver_id) {
                std::string featured_message = "Client " + std::to_string(client_id) + " says: " + message + "\n";
                boost::asio::async_write(*socket, boost::asio::buffer(featured_message),
                    [](const boost::system::error_code&, std::size_t) {});
                return;
            }
        }
    }

    void TCP::send_broadcast(const std::string& message, const std::vector<int>& excluded_clients) {
        for (const auto& player : players_) {
            const int id = player.getId();
            const auto socket = player.getSocket();
            if (std::find(excluded_clients.begin(), excluded_clients.end(), id) != excluded_clients.end())
                continue;
            std::string featured_message = message + "\n";
            boost::asio::async_write(*socket, boost::asio::buffer(featured_message),
                [](const boost::system::error_code&, std::size_t) {});
        }
    }
}