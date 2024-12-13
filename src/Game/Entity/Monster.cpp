/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Monster class
** Caroline Boilly @Aeliondw
*/

#include "Monster.hpp"


namespace Server
{
//TODO: Health is set at ten by default but it will change
    Monster::Monster(int id)
    {
        _id = id;
        _level = 1;
        _position = {0, 0};
        _health = 10;
        _damage = 10;
        _speed = 1;
        _isAlive = true;

        std::cout << "Monster " << _id << " initiated" << std::endl;
    }

    Monster::Monster(int id, int level)
    {
        _id = id;
        if (level < 1 || level > LEVEL_MAX)
            _level = 1;
        else
            _level = level;
        _position = {0, 0};
        _health = 10 * level;
        _damage = 10 * level;
        _speed = 1;
        _isAlive = true;

        std::cout << "Monster " << _id << " initiated" << std::endl;
    }

    Monster::~Monster()
    {
        std::cout << "Monster " << _id << " destroyed" << std::endl;
    }

    /*  ---- GAME LOGIC ---- */

    void Monster::shoot()
    {
        int shootSpeed = 5 * _speed;
        _shoots.emplace_back(_position.x, _position.y, shootSpeed, _damage, _direction);
    }

    void Monster::update()
    {
        for (auto &shoot : _shoots) {
            shoot.updatePosition();
        }

        _shoots.erase(std::remove_if(_shoots.begin(), _shoots.end(),
                                    [](const Shoot &shoot) { return !shoot.getIsActive(); }),
                    _shoots.end());
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
            {"pos_x", std::to_string(_position.x)},
            {"pos_y", std::to_string(_position.y)}
        };
    }

    Type Monster::getType() const
    {
        return _type;
    }

    int Monster::getSize() const
    {
        return _size;
    }

} // namespace Server
