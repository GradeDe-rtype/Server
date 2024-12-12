/*
** EPITECH PROJECT, 2024 | Caroline Boilly
** R-Type
** File description:
** Shoot class
** Caroline Boilly @Aeliondw
*/

#include "Shoot.hpp"


namespace Server {
    Shoot::Shoot(int pos_x, int pos_y, int speed, int damage, Direction direction)
    {
        _position = {pos_x, pos_y};
        _direction = direction;
        _damage = damage;
        _speed = speed;
        _isActive = true;

        std::cout << "Shoot initiated" << std::endl;
    }

    Shoot::~Shoot()
    {
        std::cout << "Shoot destroyed" << std::endl;
    }

    /*  ---- GAME LOGIC ---- */

    void Shoot::updatePosition()
    {
        if (_direction == Direction::RIGHT)
            _position.x += _speed;
        else if (_direction == Direction::LEFT)
            _position.x -= _speed;

        //TODO : Check if out of bounds (get screen width and height)
        if (_position.x < 0 || _position.x > 1920)
            _isActive = false;
    }

    /*  ---- SETTER ---- */

    void Shoot::setDamage(int damage)
    {
        if (damage < 0)
            return;
        _damage = damage;
    }

    void Shoot::setPosX(double pos_x)
    {
        _position.x = pos_x;
    }

    void Shoot::setPosY(double pos_y)
    {
        _position.y = pos_y;
    }

    void Shoot::setDirection(Direction direction)
    {
        _direction = direction;
    }

    void Shoot::setSpeed(double speed)
    {
        _speed = speed;
    }

    void Shoot::setIsActive(bool isActive)
    {
        _isActive = isActive;
    }

    /*  ---- GETTER ---- */

    int Shoot::getDamage() const
    {
        return _damage;
    }

    int Shoot::getPosX() const
    {
        return _position.x;
    }

    int Shoot::getPosY() const
    {
        return _position.y;
    }

    Direction Shoot::getDirection() const
    {
        return _direction;
    }

    double Shoot::getSpeed() const
    {
        return _speed;
    }

    bool Shoot::getIsActive() const
    {
        return _isActive;
    }

}