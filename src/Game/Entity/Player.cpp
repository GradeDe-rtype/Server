/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Player class
** Daniil Stepanov @dan13615, Caroline Boilly @Aeliondw
*/

#include "Player.hpp"

namespace Server
{
//TODO: Health is set at ten by default but it will change
    Player::Player(int id, std::shared_ptr<boost::asio::ip::tcp::socket> socket)
    {
        _id = id;
        _level = 1;
        _position = {0, 0};
        _health = 10;
        _damage = 10;
        _speed = 1;
        _isAlive = true;
        _color = 0;
        _socket = socket;
    }

    /*  ---- SETTER ---- */

    void Player::setColor(int color)
    { 
        _color = color;
    }

    /*  ---- GETTER ---- */

    std::unordered_map<std::string, int> Player::getPlayerInfo() const
    {
        return {
            {"id", _id},
            {"color", _color},
            {"health", _health},
            {"pos_x", _position.x},
            {"pos_y", _position.y}
        };
    }

    std::unordered_map<std::string, int> Player::getPlayerSmallInfo() const
    {
        return {
            {"id", _id},
            {"color", _color},
        };
    }

    int Player::getColor() const
    { 
        return _color;
    }

    bool Player::getHaveJoined() const
    { 
        return _haveJoined;
    }

    std::shared_ptr<boost::asio::ip::tcp::socket> Player::getSocket() const
    { 
        return _socket;
    }

} // namespace Server
