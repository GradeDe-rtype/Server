/*
** EPITECH PROJECT, 2024 | Caroline Boilly
** R-Type
** File description:
** Shoot class
** Caroline Boilly @Aeliondw
*/

#include "Shoot.hpp"

namespace RType
{
    namespace Game
    {
        namespace Entity
        {
            Shoot::Shoot(int pos_x, int pos_y, int speed, int damage, Direction direction)
            {
                _position = {pos_x, pos_y};
                _direction = direction;
                _damage = damage;
                _speed = speed;
                _isActive = true;
            }

            /*  ---- GAME LOGIC ---- */
            void Shoot::update()
            {
                if (_direction == Direction::RIGHT)
                    _position.x += _speed;
                else if (_direction == Direction::LEFT)
                    _position.x -= _speed;

                if (_position.x > 1920 || _position.x < 0)
                    _isActive = false;
            }

            /*  ---- SETTER ---- */
            void Shoot::setIsActive(bool isActive)
            {
                _isActive = isActive;
            }

            /*  ---- GETTER ---- */
            bool Shoot::getIsActive() const
            {
                return _isActive;
            }
        } // namespace Entity
    } // namespace Game
} // namespace RType