/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen
    >> Daniil Stepanov @dan13615

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef ROOM_HPP
    #define ROOM_HPP

#include "RType.hpp"
#include "Player.hpp"
#include "Monster.hpp"
#include "Timer.hpp"


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
            //TODO: doc
            bool _isReady = false;
            //TODO: doc
            int _score = 0;

            std::vector<Server::Player> _players;
            std::vector<Server::Monster> _monsters;
            
            Timer _monsterSpawnTimer;

        public:
            Room(int id, std::string _name);
            ~Room() = default;

            /*  ---- GAME LOGIC ---- */
            //TODO: doc
            void start();
            void addPlayer(const Player &player);
            void removePlayer(int playerId);
            void spawnMonster();
            void update();

            /*  ---- SETTER ---- */
            void setName(std::string name);
            void setMode(Mode mode);
            //TODO: doc
            void setIsReady(bool isReady);
            //TODO: doc
            void setScore(int score);

            /*  ---- GETTER ---- */
            std::unordered_map<std::string, std::string> getRoomInfo() const;
            std::string getName() const;
            int getCount() const;
            Mode getMode() const;
            //TODO: doc
            bool getIsReady() const;
            //TODO: doc
            int getScore() const;

    };
} // namespace Server

#endif // ROOM_HPP
