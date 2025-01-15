/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef RTYPE_GAME_ENTITY_HPP_
#define RTYPE_GAME_ENTITY_HPP_

namespace RType
{
    namespace Game
    {
        namespace Entity
        {
            typedef struct {
                    int x;
                    int y;
            } Position;

            typedef enum {
                LEFT,
                RIGHT,
            } Direction;

            typedef enum {
                PLAYER,
                MONSTER,
                SHOOT,
            } ENTITY_TYPE;

            typedef enum {
                MENU,
                ROOM,
                END
            } CONTEXT;
        } // namespace Entity
    } // namespace Game
} // namespace RType

#endif // RTYPE_GAME_ENTITY_HPP_