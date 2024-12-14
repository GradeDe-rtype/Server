/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include "AEntity.hpp"

namespace RType
{
    namespace Game
    {
        namespace Entity
        {
            /*  ---- SETTER ---- */
            void AEntity::setDamage(int damage)
            {
                if (damage < 0) return;
                _damage = damage;
            }

            void AEntity::setDirection(Entity::Direction direction)
            {
                _direction = direction;
            }

            void AEntity::setHealth(int health)
            {
                if (health < 0) return;
                _health = health;
            }

            void AEntity::setIsAlive(bool isAlive)
            {
                _isAlive = isAlive;
            }

            void AEntity::setLevel(int level)
            {
                if (level < 1) return;
                _level = level;
            }

            void AEntity::setPosition(Entity::Position position)
            {
                _position = position;
            }

            void AEntity::setPosX(int pos_x)
            {
                setPosition((Entity::Position){pos_x, _position.y});
            }

            void AEntity::setPosY(int pos_y)
            {
                setPosition((Entity::Position){_position.x, pos_y});
            }

            void AEntity::setSpeed(int speed)
            {
                if (speed < 0) return;
                _speed = speed;
            }


            /*  ---- GETTER ---- */
            int AEntity::getDamage() const
            {
                return _damage;
            }

            Entity::Direction AEntity::getDirection() const
            {
                return _direction;
            }

            int AEntity::getHealth() const
            {
                return _health;
            }

            int AEntity::getId() const
            {
                return _id;
            }

            bool AEntity::getIsAlive() const
            {
                return _isAlive;
            }

            int AEntity::getLevel() const
            {
                return _level;
            }

            Entity::Position AEntity::getPosition() const
            {
                return _position;
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

            /*  ---- FUNCTIONS ---- */
            void AEntity::update()
            {
            }
        } // namespace Entity
    } // namespace Game
} // namespace RType
