/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Monster class
** Caroline Boilly @Aeliondw
*/

#ifndef MONSTER_HPP
    #define MONSTER_HPP

#include "RType.hpp"


namespace Server
{
    class Monster 
    {
        private:
            typedef struct Position {
                double x;
                double y;
            } position_t;

            typedef enum {
                    LEFT,
                    DOWN,
                    RIGHT,
                    UP
            } Direction;

            int LEVEL_MAX = 10;

            int _id;
            int _level;
            int _health = 10;
            int _damage = 10;
            position_t _position;
            Direction _direction = Direction::LEFT;
            double _speed = 1;
            //TODO : Add shoot class
            //Shoot _shoot;
            //TODO : Add behavior of deplacement ?
            bool _isAlive;


        public:
            Monster(int id, int level);
            ~Monster();

            void setLevel(int level);
            void setHealth(int health);
            void setDamage(int damage);
            void setPosX(int pos_x);
            void setPosY(int pos_y);
            void setDirection(Direction direction);
            void setSpeed(double speed);
            void setIsAlive(bool isAlive);


            int getId() const;
            int getLevel() const;
            int getHealth() const;
            int getDamage() const;
            int getPosX() const;
            int getPosY() const;
            Direction getDirection() const;
            double getSpeed() const;
            bool getIsAlive() const;
    };
} // namespace Server

#endif // MONSTER_HPP
