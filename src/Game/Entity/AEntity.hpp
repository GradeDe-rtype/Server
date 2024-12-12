/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Entity abstract
** Caroline Boilly @Aeliondw
*/

#ifndef AENTITY_HPP
    #define AENTITY_HPP

    #include <RType.hpp>
    #include <IEntity.hpp>
    #include <Shoot.hpp>

namespace Server
{
    class AEntity : virtual public Server::IEntity
    {
        protected:

            int LEVEL_MAX = 2;

            int _id;
            int _level;
            position_t _position;
            Direction _direction;
            int _health;
            int _damage;
            int _speed;
            bool _isAlive;

            std::vector<Server::Shoot> _shoots;

        public:
            ~AEntity() override = default;

            void setLevel(int level) override;
            void setHealth(int health) override;
            void setPosX(int pos_x) override;
            void setPosY(int pos_y) override;
            void setDirection(Direction direction) override;
            void setSpeed(int speed) override;
            void setIsAlive(bool isAlive) override;

            int getId() const override;
            int getLevel() const override;
            int getHealth() const override;
            int getDamage() const override;
            int getPosX() const override;
            int getPosY() const override;
            Direction getDirection() const override;
            int getSpeed() const override;
            bool getIsAlive() const override;    
    };
} // namespace Server

#endif // AENTITY_HPP