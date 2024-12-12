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
    /*  ---- SETTER ---- */

    void AEntity::setLevel(int level)
    {
        if (level < 1 || level > LEVEL_MAX)
            return;
        _level = level;
    }

    void AEntity::setPosX(int pos_x)
    {
        _position.x = pos_x;
    }

    void AEntity::setPosY(int pos_y)
    {
        _position.y = pos_y;
    }

    void AEntity::setSpeed(int speed)
    {
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

    int AEntity::getSpeed() const
    {
        return _speed;
    }

    bool AEntity::getIsAlive() const
    {
        return _isAlive;
    }
}