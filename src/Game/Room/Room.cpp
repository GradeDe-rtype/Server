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

namespace RType
{
    namespace Game
    {
        std::unique_ptr<Room> Room::create(int id, const std::string &name, Server::Command *command_processor)
        {
            return std::make_unique<Room>(id, name, command_processor);
        }

        Room::Room(int id, std::string name, Server::Command *command_processor)
            : _id(id), _name(std::move(name)), _monsterTimer(1000), command_processor(command_processor)
        {
        }

        Room::Room(Room &&other) noexcept
            : _id(other._id), _name(std::move(other._name)), _mode(other._mode.load()), _isReady(other._isReady.load()), _shouldStop(other._shouldStop.load()),
              _monsterTimer(other._monsterTimer), command_processor(other.command_processor)
        {
            std::lock_guard<std::mutex> playerLock(other._playerMutex);
            std::lock_guard<std::mutex> monsterLock(other._monsterMutex);

            _players = std::move(other._players);
            _monsters = std::move(other._monsters);
        }

        // Move assignment
        Room &Room::operator=(Room &&other) noexcept
        {
            if (this != &other) {
                stop();

                _id = other._id;
                _name = std::move(other._name);
                _mode.store(other._mode.load());
                _isReady.store(other._isReady.load());
                _shouldStop.store(other._shouldStop.load());

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
            while (_players.empty()) {
                continue;
            }
            _mode.store(Mode::PLAYING);
            std::cout << "Room " << _name << " is playing" << std::endl;
            while (!_shouldStop.load()) {

                std::unique_lock<std::mutex> lock(_stateMutex);
                _stateCondVar.wait(lock, [this] {
                    return _mode.load() == Mode::PLAYING || _shouldStop.load();
                });

                if (_shouldStop.load())
                    break;

                try {
                    update();
                } catch (const std::exception &e) {
                    _mode.store(Mode::END);
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }

        bool Room::arePlayersAlive()
        {
            int DeadPlayer = 0, TotalPlayer = 0;

            for (auto &player : _players) {
                if (player.second->getIsAlive() == false)
                    DeadPlayer += 1;
                TotalPlayer += 1;
            }
            if (DeadPlayer == TotalPlayer && DeadPlayer != 0) {
                std::cout << "All players are dead" << std::endl;
                std::string arg = std::to_string(_wave);
                command_processor->process_send(-1, "end", arg);
                _mode.store(Mode::END);
                return false;
            }
            return true;
        }

        bool Room::nextWave()
        {
            if (haveAskedForNextWave == false) {
                _wave += 1;
                haveAskedForNextWave = true;
                command_processor->process_send(-1, "wave", std::to_string(_wave));
            }

            for (auto &player : _players) {
                if (player.second->getHaveJoined() == false) {
                    command_processor->to_send(player.second->getId(), std::to_string(_wave), "wave");
                    return false;
                }
            }

            for (auto &player : _players) {
                player.second->setHaveJoined(false);
            }
            haveAskedForNextWave = false;
            return true;
        }

        void Room::resetPlayers()
        {
            for (auto &player : _players) {
                player.second->setHealth(100);
                player.second->setIsAlive(true);
            }
        }

        void Room::playersUpdate()
        {
            for (auto &player : _players) {
                if (player.second->getIsAlive()) {
                    for (auto &shoot : player.second->getShoots()) {
                        shoot->update();
                        std::unordered_map<int, std::shared_ptr<Entity::Monster>> new_monsters = _monsters;
                        for (const auto &it : new_monsters) {
                            if (checkCollision(shoot->getPosition(), 1, it.second->getPosition(), it.second->getSize())) {
                                command_processor->process_send(-1, "e_death", std::to_string(it.second->getId()));
                                _monsters.erase(it.first);
                            }
                        }
                    }
                    player.second->update();
                }
            }
        }

        void Room::basicMonster(std::pair<int, std::shared_ptr<Entity::Monster>> monster)
        {
            if (monster.second->getShootTimer().hasElapsed()) {
                monster.second->shoot();
                monster.second->getShootTimer().reset();
                std::unordered_map<std::string, std::string> tmp;
                tmp["x"] = std::to_string(monster.second->getPosX());
                tmp["y"] = std::to_string(monster.second->getPosY());
                command_processor->process_send(-1, "e_shoot", rfcArgParser::CreateObject(tmp));
            }

            for (auto &shoot : monster.second->getShoots()) {
                shoot->update();
                for (auto player = _players.begin(); player != _players.end(); ++player) {
                    if (!player->second->getIsAlive())
                        continue;
                    if (checkCollision(shoot->getPosition(), 1, player->second->getPosition(), player->second->getSize())) {
                        command_processor->process_send(-1, "p_death", std::to_string(player->second->getId()));
                        player->second->setIsAlive(false);
                    }
                }
            }
        }

        void Room::kamikazeMonster(std::pair<int, std::shared_ptr<Entity::Monster>> monster)
        {
            monster.second->setPosX(monster.second->getPosX() - 5);
        }

        void Room::monstersUpdate()
        {
            int MonsterTypes[] = {Entity::Monster::BASIC_MONSTER, Entity::Monster::KAMIKAZE_MONSTER, -1};
            void (Room::*monsterUpdate[])(std::pair<int, std::shared_ptr<Entity::Monster>>) = {&Room::basicMonster, &Room::kamikazeMonster};

            for (int i = 0; MonsterTypes[i] != -1; i++) {
                for (auto &monster : _monsters) {
                    if (monster.second->getType() == MonsterTypes[i]) {
                        (this->*monsterUpdate[i])(monster);
                    }
                    for (auto player = _players.begin(); player != _players.end(); ++player) {
                        if (!player->second->getIsAlive())
                            continue;
                        if (checkCollision(monster.second->getPosition(), monster.second->getSize(), player->second->getPosition(), player->second->getSize())) {
                            command_processor->process_send(-1, "p_death", std::to_string(player->second->getId()));
                            player->second->setIsAlive(false);
                        }
                    }

                    monster.second->update();
                    std::unordered_map<std::string, std::string> tmp;
                    tmp["x"] = std::to_string(monster.second->getPosX());
                    tmp["y"] = std::to_string(monster.second->getPosY());
                    command_processor->process_send(-1, "e_position", std::to_string(monster.second->getId()) + " " + rfcArgParser::CreateObject(tmp));
                }
            }

            for (auto &_monster : _monsters) {
                if (_monster.second->getPosX() < -100) {
                    _monster.second->setPosX(900);
                    _monster.second->setPosY(std::rand() % 600);
                }
            }
        }

        void Room::update()
        {
            if (!_isReady && _mode != Mode::PLAYING)
                return;
            if (!arePlayersAlive())
                return;

            if (_monsters.empty()) {
                if (!nextWave())
                    return;
                std::cout << "Wave " << _wave << " started" << std::endl;

                if (_wave < MAX_WAVE) {
                    std::cout << "Spawning " << _wave << " monsters" << std::endl;
                    for (int i = 0; i <= _wave; i++)
                        spawnMonster();
                } else {
                    command_processor->process_send(-1, "end", std::to_string(_wave));
                    _mode.store(Mode::END);
                    return;
                }
                resetPlayers();
            }
            playersUpdate();
            monstersUpdate();
        }

        bool Room::checkCollision(const Game::Entity::Position &pos1, int size1, const Game::Entity::Position &pos2, int size2)
        {
            return (pos1.x >= pos2.x && pos1.x <= pos2.x + size2 && pos1.y >= pos2.y && pos1.y <= pos2.y + size2);
        }

        void Room::spawnMonster()
        {
            try {
                int monsterId = _monsters.size() + 1;
                auto monster = std::make_shared<Game::Entity::Monster>(monsterId, _wave);
                int type = std::rand() % 2;
                if (type == 0) {
                    monster->setType(RType::Game::Entity::Monster::KAMIKAZE_MONSTER);
                    monster->setPosX(900);
                } else {
                    monster->setType(RType::Game::Entity::Monster::BASIC_MONSTER);
                    monster->setPosX(750);
                }

                monster->setPosY(std::rand() % 500 + 50);
                std::cout << "Monster " << monsterId << " spawned at " << monster->getPosX() << ", " << monster->getPosY()
                          << "Type of " << monster->getType() << std::endl;
                std::lock_guard<std::mutex> lock(_monsterMutex);
                _monsters[monsterId] = monster;
                command_processor->process_send(-1, "enemy", rfcArgParser::CreateObject(monster->getEnemyInfo()));
            } catch (const std::exception &e) {
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
