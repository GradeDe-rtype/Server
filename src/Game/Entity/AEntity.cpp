/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Entity abstract
** Caroline Boilly @Aeliondw
*/

#include "AEntity.hpp"

namespace Server
{
    /*  ---- GAME LOGIC ---- */
    void AEntity::removeInactiveShoots()
    {
        _shoots.erase(std::remove_if(_shoots.begin(), _shoots.end(),
                                    [](const Shoot &shoot) { return !shoot.getIsActive(); }),
                                    _shoots.end());
    }

    /*  ---- SETTER ---- */

    void AEntity::setLevel(int level)
    {
        if (level < 1 || level > LEVEL_MAX)
            return;
        _level = level;
    }

    void AEntity::setHealth(int health)
    {
        if (health < 0)
            return;
        _health = health;
    }

    void AEntity::setPosX(int pos_x)
    {
        _position.x = pos_x;
    }

    void AEntity::setPosY(int pos_y)
    {
        _position.y = pos_y;
    }

    void AEntity::setDirection(Direction direction)
    {
        _direction = direction;
    }

    void AEntity::setSpeed(int speed)
    {
        if (speed < 0)
            return;
        _speed = speed;
    }

    void AEntity::setIsAlive(bool isAlive)
    {
        _isAlive = isAlive;
    }

    /*  ---- GETTER ---- */

    int AEntity::getId() const
    {
        return _id;
    }

    int AEntity::getLevel() const
    {
        return _level;
    }

    int AEntity::getHealth() const
    {
        return _health;
    }

    int AEntity::getDamage() const
    {
        return _damage;
    }

    int AEntity::getPosX() const
    {
        return _position.x;
    }

    int AEntity::getPosY() const
    {
        return _position.y;
    }

    Direction AEntity::getDirection() const
    {
        return _direction;
    }

    int AEntity::getSpeed() const
    {
        return _speed;
    }

    bool AEntity::getIsAlive() const
    {
        return _isAlive;
    }

    std::vector<Shoot> AEntity::getShoots() const
    {
        return _shoots;
    }
}