/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen
    >> Daniil Stepanov @dan13615

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include "Room.hpp"
#include <iostream>

// TODO: Send _position players to all connected player after a delay
// TODO: Send damage a player received by a monster to all player
// TODO: Send that a player die to all player

// TODO: Send the position of a player shoot to all player
// TODO: Send enemy-info to all player when a new enemy has been created

// TODO: Send _position enemy to all connected player after a delay
// TODO: Send damage a monster received by a player to all player
// TODO: Send that a monster die to all player
// TODO: Send a request to all player if they are ready to a new wave

namespace RType {
    namespace Game
    {
        // Static factory method for safe room creation
        std::unique_ptr<Room> Room::create(int id, const std::string& name, Server::Command* command_processor)
        {
            return std::make_unique<Room>(id, name, command_processor);
        }

        Room::Room(int id, std::string name, Server::Command *command_processor)
            : _id(id), _name(std::move(name)), _monsterSpawnTimer(5000), command_processor(command_processor)
        {
        }

        // Move constructor
        Room::Room(Room &&other) noexcept
            : _id(other._id), _name(std::move(other._name)), _mode(other._mode.load()), _isReady(other._isReady.load()), _shouldStop(other._shouldStop.load()),
            _monsterSpawnTimer(other._monsterSpawnTimer), command_processor(other.command_processor)
        {
            // Safely transfer players and monsters
            std::lock_guard<std::mutex> playerLock(other._playerMutex);
            std::lock_guard<std::mutex> monsterLock(other._monsterMutex);

            _players = std::move(other._players);
            _monsters = std::move(other._monsters);
        }

        // Move assignment
        Room& Room::operator=(Room&& other) noexcept
        {
            if (this != &other)
            {
                // Stop current thread if running
                stop();

                // Transfer basic data
                _id = other._id;
                _name = std::move(other._name);
                _mode.store(other._mode.load());
                _isReady.store(other._isReady.load());
                _shouldStop.store(other._shouldStop.load());

                // Safely transfer players and monsters
                std::scoped_lock lock(_playerMutex, other._playerMutex, _monsterMutex, other._monsterMutex);
                _players = std::move(other._players);
                _monsters = std::move(other._monsters);
            }
            return *this;
        }

        void Room::start()
        {
            bool expected = false;
            if (!_isReady.compare_exchange_strong(expected, true)) {
                return;
            }
            std::cout << "Room " << _name << " is ready" << std::endl;
            std::lock_guard<std::mutex> lock(_threadMutex);
            _gameThread.emplace([this]() { this->runGameLoop(); });
        }

        void Room::stop()
        {
            _shouldStop.store(true);
            _stateCondVar.notify_all();
            if (_gameThread) {
                _gameThread->join();
                _gameThread.reset();
            }
            _isReady.store(false);
            _mode.store(Mode::WAITING);
        }

        void Room::runGameLoop()
        {
            _mode.store(Mode::PLAYING);
            while (!_shouldStop.load())
            {
                std::unique_lock<std::mutex> lock(_stateMutex);
                _stateCondVar.wait(lock, [this]{
                    return _mode.load() == Mode::PLAYING || _shouldStop.load();
                });

                if (_shouldStop.load())
                    break;

                try
                {
                    update();
                }
                catch (const std::exception& e)
                {
                    _mode.store(Mode::END);
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }

        // Simplified implementations of other methods...
        void Room::update()
        {
            if (!_isReady && _mode != Mode::PLAYING)
                return;

            auto now = std::chrono::steady_clock::now();
            if (_monsterSpawnTimer.hasElapsed()) {
                spawnMonster();
                _monsterSpawnTimer.reset();
            }


            for (auto &player : _players) {
                if (player.second->getIsAlive()) {
                    for (auto &shoot : player.second->getShoots()) {
                        shoot->update();
                        for (auto it = _monsters.begin(); it != _monsters.end();) {
                            if (checkCollision(shoot->getPosition(), 1, it->second->getPosition(), it->second->getSize())) {
                                command_processor->process_send(-1, "e_death", std::to_string(it->second->getId()));
                                it = _monsters.erase(it);
                            } else
                                ++it;
                        }
                    }
                    player.second->update();
                    // TODO RFC: Send `p_position` to all players if player move
                    // TODO RFC: Send `p_shoot` to all players if player shoot
                    // TODO RFC: Send `p_damage` to all players if player take damage
                    // TODO RFC: Send `p_death` to all players if player die
                    // TODO RFC: Send `p_info` to all players if player info change

                    // std::string args = player.second->getPosInfo();
                    // command_processor->process_send(player.second->getId(), "p_position", args);
                }
            }

            for (auto &monster : _monsters) {
                for (auto &shoot : monster.second->getShoots()) {
                    shoot->update();
                    for (auto it = _players.begin(); it != _players.end();) {
                        if (checkCollision(shoot->getPosition(), 1, it->second->getPosition(), it->second->getSize())) {
                            command_processor->process_send(-1, "p_death", std::to_string(it->second->getId()));
                            it = _players.erase(it);
                        } else if (checkCollision(monster.second->getPosition(), monster.second->getSize(), it->second->getPosition(), it->second->getSize())) {
                            command_processor->process_send(-1, "p_death", std::to_string(it->second->getId()));
                            it = _players.erase(it);
                        } else
                            ++it;
                    }
                }
                monster.second->update();
                std::unordered_map<std::string, std::string> tmp;
                tmp["x"] = std::to_string(monster.second->getPosX());
                tmp["y"] = std::to_string(monster.second->getPosY());
                command_processor->process_send(-1, "e_position", std::to_string(monster.second->getId()) + " " + rfcArgParser::CreateObject(tmp));
            }

            for (auto it = _monsters.begin(); it != _monsters.end();) {
                if (it->second->getPosX() < -100) {
                    command_processor->process_send(-1, "e_death", std::to_string(it->second->getId()));
                    it = _monsters.erase(it);
                } else
                    ++it;
            }

            // TODO RFC: Send a request to all players if they are ready for a new wave
            // if (checkIfWaveReady()) {
            //     sendWaveReadyRequestToAllPlayers();
        }

        bool Room::checkCollision(const Game::Entity::Position& pos1, int size1, const Game::Entity::Position& pos2, int size2)
        {
            return (pos1.x >= pos2.x && pos1.x <= pos2.x + size2 && pos1.y >= pos2.y && pos1.y <= pos2.y + size2);
        }

        void Room::spawnMonster()
        {
            try {
                int monsterId = _monsters.size() + 1;
                int level = 1;
                auto monster = std::make_shared<Game::Entity::Monster>(monsterId, level);
                monster->setPosX(900);
                monster->setPosY(std::rand() % 600);

                std::lock_guard<std::mutex> lock(_monsterMutex);
                _monsters[monsterId] = monster;

                command_processor->process_send(-1, "enemy", rfcArgParser::CreateObject(monster->getEnemyInfo()));
            }
            catch (const std::exception& e) {
                std::cerr << "Error spawning monster: " << e.what() << std::endl;
                return;
            }
        }

        void Room::addPlayer(std::shared_ptr<Game::Entity::Player> player)
        {
            std::lock_guard<std::mutex> lock(_playerMutex);
            if ((int)_players.size() >= MAX_PLAYER) return;
            _players[player->getId()] = player;
        }

        void Room::removePlayer(int playerId)
        {
            std::lock_guard<std::mutex> lock(_playerMutex);
            _players.erase(playerId);
        }

        // Destructor
        Room::~Room()
        {
            // Ensure thread is stopped
            stop();
        }

        // Getter implementations
        std::string Room::getName() const
        {
            return _name;
        }

        int Room::getID() const
        {
            return _id;
        }

        Room::Mode Room::getMode() const
        {
            return _mode.load();
        }

        bool Room::isRunning() const
        {
            return _isReady.load() && !_shouldStop.load();
        }
    } // namespace Game
} // namespace RType
