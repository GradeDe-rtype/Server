/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen
    >> Daniil Stepanov @dan13615

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
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
                _size = 10;
                _isActive = true;
            }

            /*  ---- GAME LOGIC ---- */
            void Shoot::update()
            {
                if (_direction == Direction::RIGHT)
                    _position.x += _speed;
                else if (_direction == Direction::LEFT)
                    _position.x -= _speed;

                if (_position.x > 800 || _position.x < 0)
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