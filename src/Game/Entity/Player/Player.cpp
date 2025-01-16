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
                _damage = 25;
                _size = 40;
                _speed = 1;
                _isAlive = true;
                _color = Game::Colors::get().getColor(_id);
                _socket = socket;
                _direction = Direction::RIGHT;
            }

            void Player::shoot(int x, int y)
            {
                std::lock_guard<std::mutex> lock(_shoots_mutex);
                uint64_t new_shoot_id = s_global_shoot_id.fetch_add(1, std::memory_order_relaxed);
                auto new_shoot = std::make_shared<Shoot>(
                    new_shoot_id,
                    _id,
                    ENTITY_TYPE::PLAYER,
                    x,
                    y,
                    20,
                    _damage,
                    _direction);
                if (new_shoot) {
                    _shoots.push_back(new_shoot);
                }
            }

            void Player::update()
            {
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

            void Player::setWeapon(Player::Shoot_Type weapon)
            {
                _weapon = weapon;
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

            void Player::removeShoot(int id)
            {
                std::lock_guard<std::mutex> lock(_shoots_mutex);
                auto it = std::find_if(_shoots.begin(), _shoots.end(),
                                       [id](const std::shared_ptr<Shoot> &shoot) {
                                           return shoot && shoot->getId() == id;
                                       });

                if (it != _shoots.end()) {
                    _shoots.erase(it);
                }
            }

            std::vector<std::shared_ptr<Shoot>> Player::getShoots()
            {
                std::lock_guard<std::mutex> lock(_shoots_mutex);
                return _shoots;
            }

            Player::Shoot_Type Player::getWeapon()
            {
                return _weapon;
            }

            std::atomic<uint64_t> Player::s_global_shoot_id{0};

            bool Player::isInRoom() const
            {
                return _context == CONTEXT::ROOM;
            }

            void Player::setInRoom(bool inRoom)
            {
                _context = inRoom ? CONTEXT::ROOM : _context;
            }

            bool Player::isInMenu() const
            {
                return _context == CONTEXT::MENU;
            }

            void Player::setInMenu(bool inMenu)
            {
                _context = inMenu ? CONTEXT::MENU : _context;
            }
        } // namespace Entity
    } // namespace Game
} // namespace RType
