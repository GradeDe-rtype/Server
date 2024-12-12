/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Shoot class
** Caroline Boilly @Aeliondw
*/

#ifndef SHOOT_HPP
    #define SHOOT_HPP

#include <RType.hpp>
#include <AEntity.hpp>


namespace Server
{
    class Shoot
    {
        private:
            position_t _position;
            Direction _direction = Direction::LEFT;
            int _damage;
            int _speed;
            bool _isActive = true;

        public:
            Shoot(int pos_x, int pos_y, int speed, int damage, Direction direction);
            ~Shoot() = default;

            /*  ---- GAME LOGIC ---- */
            void updatePosition();
            // bool is_out_of_bounds(int screen_width, int screen_height);

            /*  ---- SETTER ---- */
            void setDamage(int damage);
            void setPosX(double pos_x);
            void setPosY(double pos_y);
            void setDirection(Direction direction);
            void setSpeed(double speed);
            void setIsActive(bool isActive);

            /*  ---- GETTER ---- */
            int getDamage() const;
            int getPosX() const;
            int getPosY() const;
            Direction getDirection() const;
            double getSpeed() const;
            bool getIsActive() const;
    };
} // namespace Server

#endif // SHOOT_HPP