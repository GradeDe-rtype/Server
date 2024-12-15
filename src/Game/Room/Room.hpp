/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen
    >> Daniil Stepanov @dan13615

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef RTYPE_GAME_ROOM_HPP_
#define RTYPE_GAME_ROOM_HPP_

#include <string>
#include <unordered_map>
#include "Monster.hpp"
#include "Player.hpp"
#include "Timer.hpp"

namespace RType
{
    namespace Game
    {
        class Room
        {
            public:
                enum class Mode {
                    WAITING = 0,
                    PLAYING = 1,
                    END = 2
                };

                int MAX_PLAYER = 4;

                Room(int id, std::string _name);
                ~Room() = default;

                /*  ---- GAME LOGIC ---- */
                void start();
                void addPlayer(std::shared_ptr<Game::Entity::Player> &player);
                void removePlayer(int playerId);
                void spawnMonster();
                void update();

                /*  ---- SETTER ---- */
                void setName(std::string name);
                void setMode(Mode mode);
                void setIsReady(bool isReady);
                void setScore(int score);

                /*  ---- GETTER ---- */
                std::unordered_map<std::string, std::string> getRoomInfo() const;
                std::string getName() const;
                int getCount() const;
                Mode getMode() const;
                bool getIsReady() const;
                int getScore() const;
                int getId() const;

            private:
                int _id;
                std::string _name;
                Mode _mode = Mode::WAITING;
                bool _isReady = false;
                int _score = 0;
                int _wave = 1;

                std::unordered_map<int, std::shared_ptr<Game::Entity::Player>> _players;
                std::unordered_map<int, std::shared_ptr<Game::Entity::Monster>> _monsters;

                Timer _monsterSpawnTimer;
        };
    } // namespace Game
} // namespace RType

#endif // RTYPE_GAME_ROOM_HPP_
