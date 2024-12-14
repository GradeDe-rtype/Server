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

#include "Entity.hpp"
#include "AEntity.hpp"

namespace RType
{
    namespace Game
    {
        namespace Entity
        {
            class Shoot: public AEntity
            {
                private:
                    bool _isActive = true;

                public:
                    Shoot(int pos_x, int pos_y, int speed, int damage, Entity::Direction direction);
                    ~Shoot() = default;

                    void update();

                    /*  ---- SETTER ---- */
                    void setIsActive(bool isActive);

                    /*  ---- GETTER ---- */
                    bool getIsActive() const;
            };
        } // namespace Entity
    } // namespace Game
} // namespace RType

#endif // RTYPE_GAME_ENTITY_SHOOT_HPP_