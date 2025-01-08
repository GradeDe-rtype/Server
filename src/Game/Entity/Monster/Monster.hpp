/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef RTYPE_GAME_ENTITY_MONSTER_HPP_
#define RTYPE_GAME_ENTITY_MONSTER_HPP_

#include <algorithm>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "AEntity.hpp"
#include "Shoot.hpp"

#include <Timer.hpp>

namespace RType
{
    namespace Game
    {
        namespace Entity
        {
            class Monster : public AEntity
            {
                public:
                    typedef enum {
                        MONSTER = 0,
                        BOSS = 1,
                        KAMIKAZE_MONSTER = 2,
                        BASIC_MONSTER = 3
                    } Type;

                    Monster(int id);
                    Monster(int id, int level);
                    ~Monster() override = default;

                    /*  ---- GAME LOGIC ---- */
                    void shoot();
                    void update();

                    /*  ---- SETTER ---- */
                    void setType(Type type);
                    // void setSize(int size);

                    /*  ---- GETTER ---- */
                    std::unordered_map<std::string, std::string> getEnemyInfo() const;
                    Type getType() const;
                    std::vector<std::shared_ptr<Entity::Shoot>> getShoots() const;
                    Timer &getShootTimer();

                private:
                    Type _type = Type::BASIC_MONSTER;
                    std::vector<std::shared_ptr<Shoot>> _shoots;
                    Timer _shootTimer{1000};
                    int shoot_id = 0;
            };
        } // namespace Entity
    } // namespace Game
} // namespace RType

#endif // RTYPE_GAME_ENTITY_MONSTER_HPP_
