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
    class Monster : public Server::AEntity
    {
        private:
            int _type = 0;
            int _size = 0;
            Direction _direction = Direction::LEFT;

        public:
            Monster(int id);
            Monster(int id, int level);
            ~Monster() = default;

            /*  ---- SETTER ---- */
            void setType(int type);
            void setSize(int size);
            void setDirection(Direction direction);

            /*  ---- GETTER ---- */
            std::unordered_map<std::string, int> getEnemyInfo() const;
            int getType() const;
            int getSize() const;
            Direction getDirection() const;
    };
} // namespace Server

#endif // MONSTER_HPP
