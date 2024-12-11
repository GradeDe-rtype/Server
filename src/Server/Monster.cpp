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

//TODO : Set heal, damage, speed depending on the level
// Now these characterics are set by default
    Monster::Monster(int id, int level): _id(id)
    {
        // if (level < 1 || level > LEVEL_MAX)
        //     return;
        _level = level;
        _isAlive = true;

        std::cout << "Monster " << _id << " initiated" << std::endl;
    }

    Monster::~Monster()
    {
        std::cout << "Monster " << _id << " destroyed" << std::endl;
    }

    /*  ---- GAME LOGIC ---- */

    /*  ---- SETTER ---- */

    void Monster::setLevel(int level)
    {
        if (level != _level++ || level > LEVEL_MAX)
            return;
        _level = level;
    }

    void Monster::setHealth(int health)
    {
        if (health < 0)
            return;
        _health = health;
    }

    void Monster::setDamage(int damage)
    {
        if (damage < 0)
            return;

        if (_health - damage <= 0)
            _isAlive = false;
        
        _damage = damage;
    }

    void Monster::setPosX(int pos_x)
    {
        _position.x = pos_x;
    }

    void Monster::setPosY(int pos_y)
    {
        _position.y = pos_y;
    }

    void Monster::setDirection(Direction direction)
    {
        _direction = direction;
    }

    void Monster::setSpeed(double speed)
    {
        _speed = speed;
    }

    void Monster::setIsAlive(bool isAlive)
    {
        _isAlive = isAlive;
    }

    /*  ---- GETTER ---- */

    int Monster::getId() const
    {
        return _id;
    }

    int Monster::getLevel() const
    {
        return _level;
    }

    int Monster::getHealth() const
    {
        return _health;
    }

    int Monster::getDamage() const
    {
        return _damage;
    }

    int Monster::getPosX() const
    {
        return _position.x;
    }

    int Monster::getPosY() const
    {
        return _position.y;
    }

    Monster::Direction Monster::getDirection() const
    {
        return _direction;
    }

    double Monster::getSpeed() const
    {
        return _speed;
    }

    bool Monster::getIsAlive() const
    {
        return _isAlive;
    }
    
} // namespace Server
