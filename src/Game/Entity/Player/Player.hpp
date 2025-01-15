/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Player class
** Daniil Stepanov @dan13615, Caroline Boilly @Aeliondw
*/

#ifndef RTYPE_GAME_ENTITY_PLAYER_HPP_
#define RTYPE_GAME_ENTITY_PLAYER_HPP_

#include <atomic>
#include <boost/asio.hpp>
#include <memory>
#include <mutex>
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
                    typedef enum {
                        BASIC_SHOOT = 0,
                        SHOTGUN_SHOOT = 1
                    } Shoot_Type;
                    Player(int id, std::shared_ptr<boost::asio::ip::tcp::socket> socket);
                    ~Player() = default;

                    /*  ---- GAME LOGIC ---- */
                    void shoot(int x, int y);
                    void update();

                    /*  ---- SETTER ---- */
                    void setColor(std::string color);
                    void setHaveJoined(bool haveJoined);
                    void setWeapon(Shoot_Type weapon);

                    /*  ---- GETTER ---- */
                    [[nodiscard]] std::unordered_map<std::string, std::string> getPlayerInfo() const;
                    [[nodiscard]] std::unordered_map<std::string, std::string> getPlayerSmallInfo() const;
                    [[nodiscard]] bool getHaveJoined() const;
                    [[nodiscard]] std::shared_ptr<boost::asio::ip::tcp::socket> getSocket() const;
                    [[nodiscard]] std::vector<std::shared_ptr<Shoot>> getShoots() const;
                    [[nodiscard]] std::string getPosInfo() const;
                    std::string getColor() const;
                    void removeShoot(int id);
                    std::vector<std::shared_ptr<Shoot>> getShoots();
                    Shoot_Type getWeapon();

                    bool isInRoom() const;
                    void setInRoom(bool inRoom);
                    bool isInMenu() const;
                    void setInMenu(bool inMenu);

                private:
                    CONTEXT _context = CONTEXT::MENU;
                    static std::atomic<uint64_t> s_global_shoot_id;
                    std::vector<std::shared_ptr<Shoot>> _shoots;
                    mutable std::mutex _shoots_mutex;
                    std::string _color;
                    bool _haveJoined = false;
                    std::shared_ptr<boost::asio::ip::tcp::socket> _socket;
                    Shoot_Type _weapon = BASIC_SHOOT;
            };
        } // namespace Entity
    } // namespace Game
} // namespace RType

#endif // RTYPE_GAME_ENTITY_PLAYER_HPP_
