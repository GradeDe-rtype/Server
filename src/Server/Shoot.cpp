/*
** EPITECH PROJECT, 2024 | Caroline Boilly
** R-Type
** File description:
** Shoot class
** Caroline Boilly @Aeliondw
*/

#include "Shoot.hpp"


namespace Server {
    Shoot::Shoot(double x, double y, double speed, int damage, std::string direction)
    {
        _position = {x, y};
        _speed = speed;
        _damage = damage;
        _isActive = true;
        if (direction == "right")
            _direction = Direction::RIGHT;
        else if (direction == "left")
            _direction = Direction::LEFT;
    }

    Shoot::~Shoot()
    {
        std::cout << "Shoot destroyed" << std::endl;
    }

    /*  ---- GAME LOGIC ---- */

//     def move(self):
//         if self.direction == "right":
//             self.x += self.speed
//         elif self.direction == "left":
//             self.x -= self.speed

//     def is_out_of_bounds(self, screen_width, screen_height):
//         return self.x < 0 or self.x > screen_width or self.y < 0 or self.y > screen_height

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

    Shoot::Direction Shoot::getDirection() const
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