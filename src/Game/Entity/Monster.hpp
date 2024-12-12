/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Monster class
** Caroline Boilly @Aeliondw
*/

#ifndef MONSTER_HPP
    #define MONSTER_HPP

#include <RType.hpp>
#include <AEntity.hpp>


namespace Server
{
    //TODO: Defined Type attribute with devteam
    enum class Type {
        MONSTER = 0,
        BOSS = 1,
    };
    class Monster : public Server::AEntity
    {
        private:
            //TODO: Defined Type attribute with devteam
            int _type = 0;
            int _size = 0;
            Direction _direction = Direction::LEFT;

        public:
            Monster(int id);
            Monster(int id, int level);
            ~Monster() = default;

            /*  ---- GAME LOGIC ---- */
            void shoot();
            void updatePosition();

            /*  ---- SETTER ---- */
            void setType(int type);
            void setSize(int size);

            /*  ---- GETTER ---- */
            std::unordered_map<std::string, int> getEnemyInfo() const;
            int getType() const;
            int getSize() const;
    };
} // namespace Server

#endif // MONSTER_HPP
