/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Player class
** Daniil Stepanov @dan13615, Caroline Boilly @Aeliondw
*/

#ifndef RTYPE_GAME_ENTITY_PLAYER_HPP_
#define RTYPE_GAME_ENTITY_PLAYER_HPP_

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "AEntity.hpp"
#include "Colors.hpp"
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
                    Player(int id, std::shared_ptr<boost::asio::ip::tcp::socket> socket);
                    ~Player() = default;

                    /*  ---- GAME LOGIC ---- */
                    void shoot(int x, int y);
                    void update();

                    /*  ---- SETTER ---- */
                    void setColor(std::string color);
                    void setHaveJoined(bool haveJoined);

                    /*  ---- GETTER ---- */
                    [[nodiscard]] std::unordered_map<std::string, std::string> getPlayerInfo() const;
                    [[nodiscard]] std::unordered_map<std::string, std::string> getPlayerSmallInfo() const;
                    [[nodiscard]] bool getHaveJoined() const;
                    [[nodiscard]] std::shared_ptr<boost::asio::ip::tcp::socket> getSocket() const;
                    [[nodiscard]] std::vector<std::shared_ptr<Shoot>> getShoots() const;
                    [[nodiscard]] std::string getPosInfo() const;
                    std::string getColor() const;
                    bool getIsDeadForRun();
                    void setIsDeadForRun(bool IsDeadForRestart);

                private:
                    std::string _color;
                    bool _IsDeadForRestart = false;
                    bool _haveJoined = false;
                    std::shared_ptr<boost::asio::ip::tcp::socket> _socket;
                    std::vector<std::shared_ptr<Shoot>> _shoots;
            };
        } // namespace Entity
    } // namespace Game
} // namespace RType

#endif // RTYPE_GAME_ENTITY_PLAYER_HPP_
