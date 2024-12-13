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
        _color = Color::RED;
        _socket = socket;

        std::cout << "Player " << _id << " initiated" << std::endl;
    }

    Player::~Player()
    {
        std::cout << "Player " << _id << " destroyed" << std::endl;
    }

    /*  ---- SETTER ---- */

    void Player::shoot()
    {
        int shootSpeed = 5 * _speed;
        _shoots.emplace_back(_position.x, _position.y, shootSpeed, _damage, _direction);
    }

//TODO: Check
    void Player::update()
    {
        for (auto &shoot : _shoots) {
            shoot.updatePosition();
        }

        _shoots.erase(std::remove_if(_shoots.begin(), _shoots.end(),
                                    [](const Shoot &shoot) { return !shoot.getIsActive(); }),
                    _shoots.end());
    }

    /*  ---- SETTER ---- */

    void Player::setColor(Color color)
    { 
        _color = color;
    }

    void Player::setHaveJoined(bool haveJoined)
    { 
        _haveJoined = haveJoined;
    }

    /*  ---- GETTER ---- */

    std::unordered_map<std::string, std::string> Player::getPlayerInfo() const
    {
        return {
            {"id", std::to_string(_id)},
            {"color", std::to_string(static_cast<int>(_color))},
            {"health", std::to_string(_health)},
            {"pos_x", std::to_string(_position.x)},
            {"pos_y", std::to_string(_position.y)}
        };
    }

    std::unordered_map<std::string, std::string> Player::getPlayerSmallInfo() const
    {
        return {
            {"id", std::to_string(_id)},
            {"color", std::to_string(static_cast<int>(_color))},
        };
    }

    Color Player::getColor() const
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
