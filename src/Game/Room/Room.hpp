/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Room class
** Caroline Boilly @Aeliondw
*/

#ifndef ROOM_HPP
    #define ROOM_HPP

#include <RType.hpp>
#include <Player.hpp>
#include <Monster.hpp>


namespace Server
{
    class Room
    {
        enum class Mode {
            WAITING = 0,
            PLAYING = 1,
            END = 2
        };

        int MAX_PLAYER = 4;

        private:
            int _id;
            std::string _name;
            int _count = 0;
            Mode _mode = Mode::WAITING;

            std::vector<Server::Player> _players;
            std::vector<Server::Monster> _monsters;

            //TODO: Make a Class for Timer
            std::chrono::steady_clock::time_point _lastMonsterSpawn;

        public:
            Room(int id, std::string _name);
            ~Room() = default;

            /*  ---- GAME LOGIC ---- */
            void addPlayer(const Player &player);
            void removePlayer(int playerId);
            void spawnMonster();
            void update();

            /*  ---- SETTER ---- */
            void setName(std::string name);
            void setMode(Mode mode);

            /*  ---- GETTER ---- */
            std::unordered_map<std::string, std::string> getRoomInfo() const;
            std::string getName() const;
            int getCount() const;
            Mode getMode() const;

    };
} // namespace Server

#endif // ROOM_HPP
