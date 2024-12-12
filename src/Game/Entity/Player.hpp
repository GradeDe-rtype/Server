/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Player class
** Daniil Stepanov @dan13615, Caroline Boilly @Aeliondw
*/

#ifndef PLAYER_HPP
    #define PLAYER_HPP

#include <RType.hpp>
#include <AEntity.hpp>


namespace Server
{
    enum class Color {
        RED = 0,
        GREEN = 1,
        BLUE = 2,
        YELLOW = 3,
        PINK = 4,
        CYAN = 5,
        ORANGE = 6,
        PURPLE = 7,
        WHITE = 8
    };

    class Player : public Server::AEntity
    {
        private:
            int _color;
            Direction _direction = Direction::RIGHT;
            bool _haveJoined = false;
            std::shared_ptr<boost::asio::ip::tcp::socket> _socket;

        public:
            Player(int id, std::shared_ptr<boost::asio::ip::tcp::socket> socket);
            ~Player() = default;

            /*  ---- GAME LOGIC ---- */
            void shoot();

            /*  ---- SETTER ---- */
            void setColor(int color);

            /*  ---- GETTER ---- */
            std::unordered_map<std::string, int> getPlayerInfo() const;
            std::unordered_map<std::string, int> getPlayerSmallInfo() const;
            int getColor() const;
            bool getHaveJoined() const;
            std::shared_ptr<boost::asio::ip::tcp::socket> getSocket() const;

    };
} // namespace Server

#endif // PLAYER_HPP
