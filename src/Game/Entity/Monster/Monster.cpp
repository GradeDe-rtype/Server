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
                _speed = 1;
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
                _speed = 1;
                _isAlive = true;
            }

            /*  ---- GAME LOGIC ---- */
            void Monster::shoot()
            {
                _shoots.push_back(std::make_shared<Shoot>(x, y, 15, _damage, _direction));
            }

            void Monster::update()
            {
                for (auto &shoot : _shoots)
                    shoot.updatePosition();

                _shoots.erase(std::remove_if(_shoots.begin(), _shoots.end(), [](const Shoot &shoot) { return !shoot.getIsActive(); }), _shoots.end());
            }

            /*  ---- SETTER ---- */

            void Monster::setType(Type type)
            {
                _type = type;
            }

            void Monster::setSize(int size)
            {
                _size = size;
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

            Type Monster::getType() const
            {
                return _type;
            }

            int Monster::getSize() const
            {
                return _size;
            }
        } // namespace Entity
    } // namespace Game
} // namespace RType
