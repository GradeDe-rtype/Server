/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen
    >> Daniil Stepanov @dan13615

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include "Player.hpp"
#include <iostream>

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
                _size = 40;
                _speed = 1;
                _isAlive = true;
                _color = Game::Colors::get().getColor(_id);
                _socket = socket;
                _direction = Direction::RIGHT;
            }

            /*  ---- SETTER ---- */

            void Player::shoot(int x, int y)
            {
                _shoots.push_back(std::make_shared<Shoot>(x, y, 15, _damage, _direction));
            }

            void Player::update()
            {
                _shoots.erase(std::remove_if(_shoots.begin(), _shoots.end(), [](std::shared_ptr<Shoot> shoot) { return !shoot->getIsActive(); }), _shoots.end());
            }

            /*  ---- SETTER ---- */

            void Player::setColor(std::string color)
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
                    {"color", _color},
                    {"health", std::to_string(_health)},
                    {"x", std::to_string(_position.x)},
                    {"y", std::to_string(_position.y)}};
            }

            std::unordered_map<std::string, std::string> Player::getPlayerSmallInfo() const
            {
                return {
                    {"id", std::to_string(_id)},
                    {"color", _color},
                };
            }

            std::string Player::getColor() const
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

            std::vector<std::shared_ptr<Shoot>> Player::getShoots() const
            {
                return _shoots;
            }

            std::string Player::getPosInfo() const
            {
                std::string pos = std::to_string(_id) + " {x:" + std::to_string(_position.x) + ",y:" + std::to_string(_position.y) + "}";
                return pos;
            }
        } // namespace Entity
    } // namespace Game
} // namespace RType
