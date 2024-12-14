/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef RTYPE_GAME_ENTITY_ABSTRACT_HPP_
#define RTYPE_GAME_ENTITY_ABSTRACT_HPP_

#include "Entity.hpp"
#include "IEntity.hpp"

namespace RType
{
    namespace Game
    {
        namespace Entity
        {
            class AEntity : public IEntity
            {
                protected:
                    int _id;
                    int _level;
                    Entity::Position _position;
                    Entity::Direction _direction;
                    int _health;
                    int _damage;
                    int _speed;
                    bool _isAlive;

                public:
                    AEntity() = default;
                    ~AEntity() override = default;

                    void update() override;

                    void setLevel(int level) override;
                    void setHealth(int health) override;
                    void setPosX(int pos_x) override;
                    void setPosY(int pos_y) override;
                    void setPosition(Entity::Position position) override;
                    void setDirection(Entity::Direction direction) override;
                    void setSpeed(int speed) override;
                    void setIsAlive(bool isAlive) override;

                    int getId() const override;
                    int getLevel() const override;
                    int getHealth() const override;
                    int getDamage() const override;
                    int getPosX() const override;
                    int getPosY() const override;
                    Entity::Position getPosition() const override;
                    Entity::Direction getDirection() const override;
                    int getSpeed() const override;
                    bool getIsAlive() const override;
            };
        } // namespace Entity
    } // namespace Game
} // namespace RType

#endif // RTYPE_GAME_ENTITY_ABSTRACT_HPP_