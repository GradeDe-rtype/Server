/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include "Monster.hpp"

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
                _hp = 100;
                _damage = 25;
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
            }

            /*  ---- GAME LOGIC ---- */
            void Monster::shoot()
            {
                _shoots.push_back(std::make_shared<Entity::Shoot>(_position.x, _position.y, 10, _damage, _direction));
            }

            void Monster::update()
            {
                _shoots.erase(std::remove_if(_shoots.begin(), _shoots.end(), [](std::shared_ptr<Entity::Shoot> shoot) { return !shoot->getIsActive(); }), _shoots.end());
                if (_type == Type::KAMIKAZE_MONSTER)
                    setPosX(_position.x - _speed);
            }

            /*  ---- SETTER ---- */

            void Monster::setType(Type type)
            {
                _type = type;
            }

            void Monster::setPhase(int phase)
            {
                _phase = phase;
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

            std::vector<std::shared_ptr<Shoot>> Monster::getShoots() const
            {
                return _shoots;
            }

            Timer &Monster::getShootTimer()
            {
                return _shootTimer;
            }
            int Monster::getPhase()
            {
                return _phase;
            }
            bool Monster::getRuee()
            {
                return _ruee;
            }

        } // namespace Entity
    } // namespace Game
} // namespace RType
