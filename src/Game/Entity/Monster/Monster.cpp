/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include "Monster.hpp"

#include <iostream>

namespace RType
{
    namespace Game
    {
        namespace Entity
        {
            Monster::Monster(int id)
            {
                _id = id;
                _level = 1;
                _position = {0, 0};
                _health = 100;
                _damage = 10;
                _speed = 8;
                _size = 40;
                _isAlive = true;
                _direction = Direction::LEFT;
            }

            Monster::Monster(int id, int level)
            {
                _id = id;
                if (level < 1)
                    _level = 1;
                else
                    _level = level;
                _position = {0, 0};
                _health = 100 * level;
                _damage = 10 * level;
                _speed = 8;
                _size = 40;
                _isAlive = true;
                _direction = Direction::LEFT;
            }

            /*  ---- GAME LOGIC ---- */
            void Monster::shoot() {
                std::lock_guard<std::mutex> lock(_shoots_mutex);
                uint64_t new_shoot_id = s_global_shoot_id.fetch_add(1, std::memory_order_relaxed);
                auto new_shoot = std::make_shared<Entity::Shoot>(
                    new_shoot_id,
                    _id,
                    ENTITY_TYPE::MONSTER,
                    _position.x,
                    _position.y,
                    10,
                    _damage,
                    _direction
                );
                if (new_shoot) {
                    _shoots.push_back(new_shoot);
                }
            }

            void Monster::update()
            {
                if (_type == Type::KAMIKAZE_MONSTER)
                    setPosX(_position.x - _speed);
            }

            /*  ---- SETTER ---- */

            void Monster::setType(Type type)
            {
                _type = type;
            }

            /*  ---- GETTER ---- */
            std::unordered_map<std::string, std::string> Monster::getEnemyInfo() const
            {
                return {
                    {"id", std::to_string(_id)},
                    {"type", std::to_string(static_cast<int>(_type))},
                    {"size", std::to_string(_size)},
                    {"health", std::to_string(_health)},
                    {"x", std::to_string(_position.x)},
                    {"y", std::to_string(_position.y)}};
            }

            Monster::Type Monster::getType() const
            {
                return _type;
            }

            std::vector<std::shared_ptr<Shoot>> Monster::getShoots() {
                std::lock_guard<std::mutex> lock(_shoots_mutex);
                return _shoots;
            }

            Timer &Monster::getShootTimer()
            {
                return _shootTimer;
            }

            void Monster::removeShoot(int id) {
                std::lock_guard<std::mutex> lock(_shoots_mutex);
                auto it = std::find_if(_shoots.begin(), _shoots.end(),
                    [id](const std::shared_ptr<Shoot>& shoot) {
                        return shoot && shoot->getId() == id;
                    });
                if (it != _shoots.end()) {
                    _shoots.erase(it);
                } else {
                    std::cout << "Attempt to remove non-existent shoot (ID: " << id
                              << ") from monster " << _id << std::endl;
                }
            }

            std::atomic<uint64_t> Monster::s_global_shoot_id{0};
        } // namespace Entity
    } // namespace Game
} // namespace RType
