/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Player class
** Daniil Stepanov @dan13615, Caroline Boilly @Aeliondw
*/

#ifndef RTYPE_GAME_ENTITY_INTERFACE_HPP_
#define RTYPE_GAME_ENTITY_INTERFACE_HPP_

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "AEntity.hpp"
#include "Shoot.hpp"

namespace RType
{
    namespace Game
    {
        namespace Entity
        {
            class Player : public AEntity
            {
                public:
                    typedef enum {
                        RED = 0,
                        GREEN = 1,
                        BLUE = 2,
                        YELLOW = 3,
                        PINK = 4,
                        CYAN = 5,
                        ORANGE = 6,
                        PURPLE = 7,
                        WHITE = 8
                    } Color;

                    Player(int id, std::shared_ptr<boost::asio::ip::tcp::socket> socket);
                    ~Player() = default;

                    /*  ---- GAME LOGIC ---- */
                    void shoot(int x, int y);
                    void update();

                    /*  ---- SETTER ---- */
                    void setColor(Color color);
                    void setHaveJoined(bool haveJoined);

                    /*  ---- GETTER ---- */
                    std::unordered_map<std::string, std::string> getPlayerInfo() const;
                    std::unordered_map<std::string, std::string> getPlayerSmallInfo() const;
                    Color getColor() const;
                    bool getHaveJoined() const;
                    std::shared_ptr<boost::asio::ip::tcp::socket> getSocket() const;

                private:
                    Color _color;
                    bool _haveJoined = false;
                    std::shared_ptr<boost::asio::ip::tcp::socket> _socket;
                    std::vector<std::shared_ptr<Shoot>> _shoots;
            };
        } // namespace Entity
    } // namespace Game
} // namespace RType

#endif // RTYPE_GAME_ENTITY_INTERFACE_HPP_
