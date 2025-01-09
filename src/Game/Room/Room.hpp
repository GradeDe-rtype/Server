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

#define MAX_WAVE 6
#define MAX_PLAYER 4

#include <atomic>
#include <condition_variable>
#include <future>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include "Command.hpp"
#include "Monster.hpp"
#include "Player.hpp"
#include "Server.hpp"
#include "Timer.hpp"

namespace Server
{
    class Command;
}

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

                // Constructor using std::unique_ptr for thread ownership
                static std::unique_ptr<Room> create(int id, const std::string &name, Server::Command *command_processor);
                Room(int id, std::string name, Server::Command *command_processor);

                // Destructor
                ~Room();

                // Prevent copying
                Room(const Room &) = delete;
                Room &operator=(const Room &) = delete;

                // Allow moving
                Room(Room &&other) noexcept;
                Room &operator=(Room &&other) noexcept;

                // Game Logic Methods
                void start();
                void stop();
                void addPlayer(std::shared_ptr<Game::Entity::Player> player);
                void removePlayer(int playerId);
                void update();
                bool checkCollision(const Game::Entity::Position &pos1, int size1, const Game::Entity::Position &pos2, int size2);
                bool arePlayersAlive();
                bool nextWave();
                void resetPlayers();
                void Monster_death(const std::pair<int, std::shared_ptr<Entity::Monster>> &it);
                void Player_death(const std::pair<int, std::shared_ptr<Entity::Player>> &it);
                void playersUpdate();
                void basicMonster(std::pair<int, std::shared_ptr<Entity::Monster>> monster);
                void kamikazeMonster(std::pair<int, std::shared_ptr<Entity::Monster>> monster);
                void monstersUpdate();
                void shootsUpdate();

                // Setters
                void setMode(Mode mode);
                void setIsReady(bool isReady);

                // Getters
                std::string getName() const;
                int getID() const;
                Mode getMode() const;
                bool isRunning() const;

            private:
                // Private constructor to enforce using create() method
                Room(int id, std::string name);

                // Core room data
                int _id;
                std::string _name;
                int _wave = 0;
                bool haveAskedForNextWave = false;
                std::atomic<Mode> _mode{Mode::WAITING};
                std::atomic<bool> _isReady{false};
                std::atomic<bool> _shouldStop{false};

                // Thread-safe collections
                std::mutex _playerMutex;
                std::unordered_map<int, std::shared_ptr<Game::Entity::Player>> _players;

                std::mutex _monsterMutex;
                std::unordered_map<int, std::shared_ptr<Game::Entity::Monster>> _monsters;

                // Thread management
                std::optional<std::jthread> _gameThread;
                std::mutex _threadMutex;

                // Synchronization primitives
                std::condition_variable _stateCondVar;
                mutable std::mutex _stateMutex;

                // Internal methods
                void runGameLoop();
                void spawnMonster();

                Timer _monsterTimer;

                Server::Command *command_processor;
        };
    } // namespace Game
} // namespace RType
#endif
