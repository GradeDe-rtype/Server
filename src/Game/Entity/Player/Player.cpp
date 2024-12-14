/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Player class
** Daniil Stepanov @dan13615, Caroline Boilly @Aeliondw
*/

#include "Player.hpp"

namespace RType
{
    namespace Game
    {
        namespace Entity
        {
            Player::Player(int id, std::shared_ptr<boost::asio::ip::tcp::socket> socket)
            {
                _id = id;
                _level = 1;
                _position = {0, 0};
                _health = 100;
                _damage = 10;
                _speed = 1;
                _isAlive = true;
                _color = Color::RED;
                _socket = socket;
            }

            /*  ---- SETTER ---- */

            void Player::shoot(int x, int y)
            {
                _shoots.push_back(std::make_shared<Shoot>(x, y, 15, _damage, _direction));
            }

            void Player::update()
            {
                for (auto &shoot : _shoots)
                    shoot->update();

                _shoots.erase(std::remove_if(_shoots.begin(), _shoots.end(), [](const Shoot &shoot) { return !shoot.getIsActive(); }), _shoots.end());
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
                    {"x", std::to_string(_position.x)},
                    {"y", std::to_string(_position.y)}};
            }

            std::unordered_map<std::string, std::string> Player::getPlayerSmallInfo() const
            {
                return {
                    {"id", std::to_string(_id)},
                    {"color", std::to_string(static_cast<int>(_color))},
                };
            }

            Player::Color Player::getColor() const
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
        } // namespace Entity
    } // namespace Game
} // namespace RType
