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
            Shoot::Shoot(int id, int related_id, ENTITY_TYPE related_entity, int pos_x, int pos_y, int speed, int damage, Direction direction)
            {
                _id = id;
                _related_id = related_id;
                _related_entity = related_entity;
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

            std::unordered_map<std::string, std::string> Shoot::getShootInfo() const
            {
                std::unordered_map<std::string, std::string> data;
                data["id"] = std::to_string(_id);
                data["from"] = ENTITY_TYPE::PLAYER == _related_entity ? "player" : "monster";
                data["related_id"] = std::to_string(_related_id);
                return data;
            }
        } // namespace Entity
    } // namespace Game
} // namespace RType