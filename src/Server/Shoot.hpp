/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Shoot class
** Caroline Boilly @Aeliondw
*/

#ifndef SHOOT_HPP
    #define SHOOT_HPP

#include "RType.hpp"


namespace Server
{
    class Shoot
    {
        private:
            typedef struct Position {
                double x;
                double y;
            } position_t;

            typedef enum {
                    LEFT,
                    RIGHT,
            } Direction;

            int _damage;
            position_t _position;
            Direction _direction;
            double _speed;
            bool _isActive = true;

        public:
            Shoot(double x, double y, double speed, int damage, std::string direction);
            ~Shoot();

            // void move();
            // bool is_out_of_bounds(int screen_width, int screen_height);

            void setDamage(int damage);
            void setPosX(double pos_x);
            void setPosY(double pos_y);
            void setDirection(Direction direction);
            void setSpeed(double speed);
            void setIsActive(bool isActive);

            int getDamage() const;
            int getPosX() const;
            int getPosY() const;
            Direction getDirection() const;
            double getSpeed() const;
            bool getIsActive() const;
    };
} // namespace Server

#endif // SHOOT_HPP