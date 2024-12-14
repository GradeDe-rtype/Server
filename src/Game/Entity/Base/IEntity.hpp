/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef RTYPE_GAME_ENTITY_INTERFACE_HPP_
#define RTYPE_GAME_ENTITY_INTERFACE_HPP_

#include <vector>
#include "Entity.hpp"

namespace RType
{
    namespace Game
    {
        namespace Entity
        {
            class IEntity
            {
                public:
                    virtual ~IEntity() = default;

                    virtual void update() = 0;

                    virtual void setDamage(int damage) = 0;
                    virtual void setDirection(Entity::Direction direction) = 0;
                    virtual void setHealth(int health) = 0;
                    virtual void setIsAlive(bool isAlive) = 0;
                    virtual void setLevel(int level) = 0;
                    virtual void setPosition(Entity::Position position) = 0;
                    virtual void setPosX(int pos_x) = 0;
                    virtual void setPosY(int pos_y) = 0;
                    virtual void setSize(int size) = 0;
                    virtual void setSpeed(int speed) = 0;

                    virtual int getDamage() const = 0;
                    virtual Entity::Direction getDirection() const = 0;
                    virtual int getHealth() const = 0;
                    virtual int getId() const = 0;
                    virtual bool getIsAlive() const = 0;
                    virtual int getLevel() const = 0;
                    virtual Entity::Position getPosition() const = 0;
                    virtual int getPosX() const = 0;
                    virtual int getPosY() const = 0;
                    virtual int getSize() const = 0;
                    virtual int getSpeed() const = 0;
            };
        } // namespace Entity
    } // namespace Game
} // namespace RType

#endif // RTYPE_GAME_ENTITY_INTERFACE_HPP_