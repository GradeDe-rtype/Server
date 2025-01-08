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

namespace RType
{
    namespace Game
    {
        // Static factory method for safe room creation
        std::unique_ptr<Room> Room::create(int id, const std::string &name, Server::Command *command_processor)
        {
            return std::make_unique<Room>(id, name, command_processor);
        }

        Room::Room(int id, std::string name, Server::Command *command_processor)
            : _id(id), _name(std::move(name)), _monsterTimer(1000), command_processor(command_processor)
        {
        }

        // Move constructor
        Room::Room(Room &&other) noexcept
            : _id(other._id), _name(std::move(other._name)), _mode(other._mode.load()), _isReady(other._isReady.load()), _shouldStop(other._shouldStop.load()),
              _monsterTimer(other._monsterTimer), command_processor(other.command_processor)
        {
            // Safely transfer players and monsters
            std::lock_guard<std::mutex> playerLock(other._playerMutex);
            std::lock_guard<std::mutex> monsterLock(other._monsterMutex);

            _players = std::move(other._players);
            _monsters = std::move(other._monsters);
        }

        // Move assignment
        Room &Room::operator=(Room &&other) noexcept
        {
            if (this != &other) {
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

        void Room::update()
        {
            int DeadPlayer = 0, TotalPlayer = 0;
            if (!_isReady && _mode != Mode::PLAYING)
                return;

            for (auto &player : _players) {
                if(player.second->getIsDeadForRun() == true) {
                    std::cout << "Spawning " << _wave << " monsters" << std::endl;
                    for (int i = 0; i <= _wave; i++)
                        spawnMonster();
                    player.second->setIsDeadForRun(false);
                }
            }

            for (auto &player : _players) {
                if (player.second->getIsAlive() == false)
                    DeadPlayer += 1;
                TotalPlayer += 1;
            }

            if (DeadPlayer == TotalPlayer && DeadPlayer != 0) {
                std::cout << "All players are dead" << std::endl;
                std::string arg = std::to_string(_wave);
                command_processor->send(-1, "end", arg);
                _isReady = false;
                _mode = Mode::WAITING;
                _wave = 1;
                for (auto &player : _players) {
                    player.second->setHealth(100);
                    player.second->setIsAlive(true);
                }
                // _mode.store(Mode::END);
                return;
            }
            if (_monsters.empty() || (_monsters.begin()->second->getType() == Entity::Monster::BOSS)) {
                if (!_monsters.empty()) {
                    if (_monsters.begin()->second->getIsAlive()) {
                        if(_monsters.begin()->second->getPhase() == 1 && _monsters.begin()->second->getHp() > 50)
                            return;
                        if(_monsters.begin()->second->getPhase() == 2 && _monsters.begin()->second->getHp() > 25)
                            return;
                        if(_monsters.begin()->second->getPhase() == 3 && _monsters.begin()->second->getHp() > 0)
                            return;
                        if(_monsters.begin()->second->getHp() < 0)
                            std::cout << "verif" << std::endl;
                    }
                }

                if (haveAskedForNextWave == false) {
                    _wave += 1;
                    haveAskedForNextWave = true;
                    command_processor->send(-1, "wave", std::to_string(_wave));
                }

                for (auto &player : _players) {
                    if (player.second->getHaveJoined() == false) {
                        command_processor->to_send(player.second->getId(), std::to_string(_wave), "wave");
                        return;
                    }
                }

                for (auto &player : _players) {
                    player.second->setHaveJoined(false);
                }
                haveAskedForNextWave = false;

                std::cout << "Wave " << _wave << " started" << std::endl;

                if (_wave < 5) {
                    // spawn the number of monster that match with the nbr of wave

                    std::cout << "Spawning " << _wave << " monsters" << std::endl;
                    for (int i = 0; i <= _wave; i++)
                        spawnMonster();
                } else if (_wave < 8 ) {
                    if (_wave == 5) {
                        spawnBoss();
                        std::cout << "Spawning Boss" << std::endl;
                    }
                    if (_monsters.begin()->second->getHp() <= 50 && _monsters.begin()->second->getHp() > 25) {
                        _monsters.begin()->second->setPhase(2);
                        _monsters.begin()->second->setPosX(900);
                    } else if (_monsters.begin()->second->getHp() <= 25) {
                        _monsters.begin()->second->setPhase(3);
                        _monsters.begin()->second->setPosX(900);
                    }

                    //ici
                    
                } else {
                    // win and leave
                    std::string arg = std::to_string(_wave);
                    command_processor->send(-1, "end", arg);
                    _mode.store(Mode::END);
                    return;
                }
                for (auto &player : _players) {
                    player.second->setHealth(100);
                    player.second->setIsAlive(true);
                    player.second->setIsDeadForRun(true);
                }
            }

            //todo : faire dans l'abstracte de la classe entité damageTaken a la cre du 
            // mob ou player, et au oins si il se fait hit j'appel juste la fonction got hit qui en fonction de l'entité prendra plus ou moins de degats
            //ajouter aussi des hp aux mobs et players
            //faire en sorte que si on passe sous les diff stage d'hp, faire une demande, envoyer le numero de wave au client 
            //remettre le boss a 900 et le faire revenir que si les gens ont accpter et confimé
            for (auto &player : _players) {
                if (player.second->getIsAlive()) {
                    for (auto &shoot : player.second->getShoots()) {
                        shoot->update();
                        std::unordered_map<int, std::shared_ptr<Entity::Monster>> new_monsters = _monsters;
                        for (const auto &it : new_monsters) {
                            if (checkCollision(shoot->getPosition(), 1, it.second->getPosition(), it.second->getSize())) {
                                it.second->TakeDamage(100);
                                if (it.second->getHp() <= 0)
                                    it.second->setIsAlive(false);
                                if (!it.second->getIsAlive()) {
                                    
                                    command_processor->send(-1, "e_death", std::to_string(it.second->getId()));
                                    _monsters.erase(it.first);
                                } 
                            }
                        }
                    }
                    player.second->update();
                }
            }

            for (auto &monster : _monsters) {
                //boss
                if (monster.second->getType() == Entity::Monster::BOSS) {                    
                    if (monster.second->getPosX() <= 0) {
                        monster.second->setPosX(900);
                        monster.second->setRuee(false);
                    }

                    if (monster.second->getPosX() <= 750) {
                        if (monster.second->getHp() > 50) {
                            if (monster.second->getShootTimer().hasElapsed()) {
                                monster.second->shoot();
                                monster.second->getShootTimer().reset();
                                command_processor->send(-1, "e_shoot", rfcArgParser::CreateObject({
                                    {"x", std::to_string(monster.second->getPosX())},
                                    {"y", std::to_string(monster.second->getPosY())}
                                }));
                            }
                            if (monster.second->getSpawnTimer().hasElapsed()) {
                                spawnMonster();
                                monster.second->getSpawnTimer().reset();
                            }
                        } else if (monster.second->getHp() > 25) {
                            if (monster.second->getShootTimer().hasElapsed()) {
                                monster.second->shoot();
                                monster.second->getShootTimer().reset();
                                command_processor->send(-1, "e_shoot", rfcArgParser::CreateObject({
                                    {"x", std::to_string(monster.second->getPosX())},
                                    {"y", std::to_string(monster.second->getPosY())}
                                }));
                            }
                            if (monster.second->getRushTimer().hasElapsed() && monster.second->getRuee() == false) {
                                int newY = (std::rand() % 2 == 0) ? 200 : 400;
                                monster.second->setPosY(newY);
                                monster.second->setPosX(monster.second->getPosX() - 8);
                                monster.second->getRushTimer().reset();
                                monster.second->setRuee(true);
                            }
                        } else {
                            if (monster.second->getRushTimer().hasElapsed() && monster.second->getRuee() == false) {
                                int newY = (std::rand() % 2 == 0) ? 200 : 400;
                                monster.second->setPosY(newY);
                                monster.second->setPosX(monster.second->getPosX() - 8);
                                monster.second->getRushTimer().reset();
                                monster.second->setRuee(true);
                            } else {
                                if (monster.second->getShootTimer().hasElapsed()) {
                                    monster.second->shoot();
                                    monster.second->getShootTimer().reset();
                                    command_processor->send(-1, "e_shoot", rfcArgParser::CreateObject({
                                        {"x", std::to_string(monster.second->getPosX())},
                                        {"y", std::to_string(monster.second->getPosY())}
                                    }));
                                }
                                if (monster.second->getSpawnTimer().hasElapsed()) {
                                    spawnMonster();
                                    monster.second->getSpawnTimer().reset();
                                }
                            }
                        }
                    }
                }

                // If bot is basic then shoot
                if (monster.second->getType() == Entity::Monster::BASIC_MONSTER) {
                    if (monster.second->getShootTimer().hasElapsed()) {
                        monster.second->shoot();
                        monster.second->getShootTimer().reset();
                        std::unordered_map<std::string, std::string> tmp;
                        tmp["x"] = std::to_string(monster.second->getPosX());
                        tmp["y"] = std::to_string(monster.second->getPosY());
                        command_processor->send(-1, "e_shoot", rfcArgParser::CreateObject(tmp));
                    }

                    for (auto &shoot : monster.second->getShoots()) {
                        shoot->update();
                        for (auto player = _players.begin(); player != _players.end(); ++player) {
                            if (!player->second->getIsAlive())
                                continue;
                            if (checkCollision(shoot->getPosition(), 1, player->second->getPosition(), player->second->getSize())) {
                                player->second->TakeDamage(25);
                                if (player->second->getHp() <= 0)
                                    player->second->setIsAlive(false);
                                if (!player->second->getIsAlive()) {
                                    command_processor->send(-1, "p_death", std::to_string(player->second->getId()));
                                    player->second->setIsAlive(false);
                                } 
                            }
                        }
                    }
                }

                // Verification of collision for all type of ot
                if (monster.second->getType() == Entity::Monster::KAMIKAZE_MONSTER)
                    monster.second->setPosX(monster.second->getPosX() - 5);
                if (monster.second->getType() == Entity::Monster::BOSS) {
                    std::cout << "caca " << monster.second->getPosX() <<std::endl;
                    if (monster.second->getPosX() > 750 && monster.second->getPosX() <= 900)
                        monster.second->setPosX(monster.second->getPosX() - 3);
                    if (monster.second->getRuee() == true)
                        monster.second->setPosX(monster.second->getPosX() - 8);
                }
                for (auto player = _players.begin(); player != _players.end(); ++player) {
                    if (!player->second->getIsAlive())
                        continue;
                    if (checkCollision(monster.second->getPosition(), monster.second->getSize(), player->second->getPosition(), player->second->getSize())) {
                        player->second->TakeDamage(25);
                        if (player->second->getHp() <= 0)
                            player->second->setIsAlive(false);
                        if (!player->second->getIsAlive()) {
                            command_processor->send(-1, "p_death", std::to_string(player->second->getId()));
                            player->second->setIsAlive(false);
                        } 
                    }
                }

                monster.second->update();
                std::unordered_map<std::string, std::string> tmp;
                tmp["x"] = std::to_string(monster.second->getPosX());
                tmp["y"] = std::to_string(monster.second->getPosY());
                command_processor->send(-1, "e_position", std::to_string(monster.second->getId()) + " " + rfcArgParser::CreateObject(tmp));
            }

            for (auto &_monster : _monsters) {
                if (_monster.second->getPosX() < -100) {
                    _monster.second->setPosX(900);
                    _monster.second->setPosY(std::rand() % 600);
                }
            }
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
                    monster->setPosX(800);
                }

                monster->setPosY(std::rand() % 600);
                monster->setHp(100);
                std::cout << "Monster " << monsterId << " spawned at " << monster->getPosX() << ", " << monster->getPosY()
                          << "Type of " << monster->getType() << std::endl;
                std::lock_guard<std::mutex> lock(_monsterMutex);
                _monsters[monsterId] = monster;
                command_processor->send(-1, "enemy", rfcArgParser::CreateObject(monster->getEnemyInfo()));
            } catch (const std::exception &e) {
                std::cerr << "Error spawning monster: " << e.what() << std::endl;
                return;
            }
        }

        void Room::spawnBoss()
        {
            try {
                int monsterId = _monsters.size() + 10;
                auto monster = std::make_shared<Game::Entity::Monster>(monsterId, _wave);
                monster->setType(RType::Game::Entity::Monster::BOSS);
                monster->setHp(1000);
                monster->setDamage(50);
                monster->setPosY(300);
                monster->setPosX(700);
                monster->setPhase(1);

                std::cout << "Monster " << monsterId << " spawned at " << monster->getPosX() << ", " << monster->getPosY()
                          << "Type of " << monster->getType() << std::endl;
                std::lock_guard<std::mutex> lock(_monsterMutex);
                _monsters[monsterId] = monster;
                command_processor->send(-1, "enemy", rfcArgParser::CreateObject(monster->getEnemyInfo()));
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

