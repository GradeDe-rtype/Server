/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen
    >> Daniil Stepanov @dan13615

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef RTYPE_GAME_ENTITY_SHOOT_HPP_
#define RTYPE_GAME_ENTITY_SHOOT_HPP_

#include <string>
#include <unordered_map>
#include "AEntity.hpp"
#include "Entity.hpp"

namespace RType
{
    namespace Game
    {
        namespace Entity
        {
            class Shoot : public AEntity
            {
                public:


                private:
                    bool _isActive = true;
                    int _id = 0;
                    int _related_id = 0;
                    ENTITY_TYPE _related_entity;

                public:
                    Shoot(int id, int related_id, ENTITY_TYPE related_entity, int pos_x, int pos_y, int speed, int damage, Entity::Direction direction);
                    ~Shoot() = default;

                    void update();

                    /*  ---- SETTER ---- */
                    void setIsActive(bool isActive);

                    /*  ---- GETTER ---- */
                    bool getIsActive() const;
                    std::unordered_map<std::string, std::string> Shoot::getShootInfo() const;
            };
        } // namespace Entity
    } // namespace Game
} // namespace RType

#endif // RTYPE_GAME_ENTITY_SHOOT_HPP_