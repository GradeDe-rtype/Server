/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Entity abstract
** Caroline Boilly @Aeliondw
*/

#include "AEntity.hpp"

namespace RType
{
    namespace Game
    {
        namespace Entity
        {
            /*  ---- SETTER ---- */
            void AEntity::setLevel(int level)
            {
                if (level < 1) return;
                _level = level;
            }

            void AEntity::setHealth(int health)
            {
                if (health < 0) return;
                _health = health;
            }

            void AEntity::setPosX(int pos_x)
            {
                setPosition((Entity::Position){pos_x, _position.y})
            }

            void AEntity::setPosY(int pos_y)
            {
                setPosition((Entity::Position){_position.x, pos_y})
            }

            void AEntity::setPosition(Entity::Position position)
            {
                _position = position;
            }

            void AEntity::setDirection(Entity::Direction direction)
            {
                _direction = direction;
            }

            void AEntity::setSpeed(int peed)
            {
                if (speed < 0) return;
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

            Entity::Position AEntity::getPosition() const
            {
                return _position;
            }

            Entity::Direction AEntity::getDirection() const
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

            /*  ---- FUNCTIONS ---- */
            void AEntity::update()
            {
            }
        } // namespace Entity
    } // namespace Game
} // namespace RType
