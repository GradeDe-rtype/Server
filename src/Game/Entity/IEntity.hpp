/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Entity interface
** Caroline Boilly @Aeliondw
*/

#ifndef IENTITY_HPP
    #define IENTITY_HPP

#include <RType.hpp>


namespace Server
{
    class IEntity
    {
        public:
            virtual ~IEntity() = default;

            virtual void setLevel(int level) = 0;
            virtual void setPosX(int pos_x) = 0;
            virtual void setPosY(int pos_y) = 0;
            virtual void setSpeed(int speed) = 0;
            virtual void setIsAlive(bool isAlive) = 0;

            virtual int getId() const = 0;
            virtual int getLevel() const = 0;
            virtual int getHealth() const = 0;
            virtual int getDamage() const = 0;
            virtual int getPosX() const = 0;
            virtual int getPosY() const = 0;
            virtual int getSpeed() const = 0;
            virtual bool getIsAlive() const = 0;
    };
} // namespace Server

#endif // IENTITY_HPP