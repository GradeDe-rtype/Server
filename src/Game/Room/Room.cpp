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
#include <utility>

namespace RType
{
    namespace Game
    {
        std::unique_ptr<Room> Room::create(int id, const std::string &name, Server::Command *command_processor)
        {
            return std::make_unique<Room>(id, name, command_processor);
        }

        Room::Room(int id, std::string name, Server::Command *command_processor)
            : _id(id), _name(std::move(name)), command_processor(command_processor)
        {
        }

        Room::Room(Room &&other) noexcept
            : _id(other._id), _name(std::move(other._name)), _mode(other._mode.load()), _isReady(other._isReady.load()), _shouldStop(other._shouldStop.load()),
              command_processor(other.command_processor)
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

                if (_mode.load() == Mode::END) {
                    for (auto &player : _players)
                        player.second->setDefault();
                    stop();
                    break;
                }

                try {
                    update();
                } catch (const std::exception &e) {
                    _mode.store(Mode::END);
                    break;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }

        std::vector<int> Room::getPlayersId()
        {
            std::vector<int> playersId;
            for (const auto &player : _players) {
                playersId.push_back(player.first);
            }
            return playersId;
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
                command_processor->send(getPlayersId(), "end", "lost");
                for (auto &player : _players)
                    player.second->setDefault();
                _mode.store(Mode::END);
                _stateCondVar.notify_all();
                return false;
            }
            return true;
        }

        bool Room::nextWave()
        {
            if (haveAskedForNextWave == false) {
                _wave += 1;
                haveAskedForNextWave = true;
                command_processor->send(getPlayersId(), "wave", std::to_string(_wave));
            }

            for (auto &player : _players) {
                if (player.second->getHaveJoined() == false) {
                    command_processor->to_send(player.second->getId(), "wave", std::to_string(_wave));
                    player.second->setWeapon(RType::Game::Entity::Player::Shoot_Type::BASIC_SHOOT);
                    return false;
                }
            }

            for (auto &player : _players)
                player.second->setHaveJoined(false);
            haveAskedForNextWave = false;
            return true;
        }

        void Room::resetPlayers()
        {
            for (auto &player : _players) {
                player.second->setHealth(100);
                player.second->setIsAlive(true);
                player.second->setWeapon(Entity::Player::Shoot_Type::BASIC_SHOOT);
            }
        }

        void Room::killAllMonsters(void)
        {
            for (auto &monster : _monsters) {
                for (auto &shoot : monster.second->getShoots()) {
                    shoot->setIsActive(false);
                    std::unordered_map<std::string, std::string> mess = shoot->getShootInfo();
                    command_processor->send(getPlayersId(), "s_death", rfcArgParser::CreateObject(mess));
                    monster.second->removeShoot(shoot->getId());
                }
                command_processor->send(getPlayersId(), "e_death", std::to_string(monster.second->getId()));
                _monsters.erase(monster.first);
            }
        }

        void Room::Monster_death(const std::pair<int, std::shared_ptr<Entity::Monster>> &it)
        {
            auto Mshoots = it.second->getShoots();
            for (auto &Mshoot : Mshoots) {
                Mshoot->setIsActive(false);
                std::unordered_map<std::string, std::string> mess = Mshoot->getShootInfo();
                command_processor->send(getPlayersId(), "s_death", rfcArgParser::CreateObject(mess));
                it.second->removeShoot(Mshoot->getId());
            }
            command_processor->send(getPlayersId(), "e_death", std::to_string(it.second->getId()));
            _monsters.erase(it.first);
        }

        void Room::Player_death(const std::pair<int, std::shared_ptr<Entity::Player>> &it)
        {
            command_processor->send(getPlayersId(), "p_death", std::to_string(it.second->getId()));
            it.second->setIsAlive(false);
            auto Pshoots = it.second->getShoots();
            for (auto &Pshoot : Pshoots) {
                Pshoot->setIsActive(false);
                std::unordered_map<std::string, std::string> mess = Pshoot->getShootInfo();
                command_processor->send(getPlayersId(), "s_death", rfcArgParser::CreateObject(mess));
                it.second->removeShoot(Pshoot->getId());
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
                                it.second->TakeDamage(player.second->getDamage());
                                command_processor->send(getPlayersId(), "s_death", rfcArgParser::CreateObject(shoot->getShootInfo()));
                                player.second->removeShoot(shoot->getId());
                                command_processor->send(getPlayersId(), "e_damage", std::to_string(it.second->getId()) + " " + std::to_string(player.second->getDamage()));
                                if (it.second->getHealth() <= 0)
                                    Monster_death(it);
                            }
                        }
                    }
                    std::unordered_map<int, std::shared_ptr<Entity::Monster>> new_monsters = _monsters;
                    for (const auto &monster : new_monsters) {
                        if (checkCollision(player.second->getPosition(), player.second->getSize(), monster.second->getPosition(), monster.second->getSize())) {
                            if (monster.second->getType() == Entity::Monster::HEALTH_BONUS) {
                                if (player.second->getHealth() + 25 > 100)
                                    player.second->setHealth(100);
                                else
                                    player.second->setHealth(player.second->getHealth() + 25);
                                command_processor->send(getPlayersId(), "p_info", rfcArgParser::CreateObject(player.second->getPlayerInfo()));
                                Monster_death(monster);
                            } else if (monster.second->getType() == Entity::Monster::DAMAGE_BONUS) {
                                player.second->setDamage(player.second->getDamage() + 10);
                                command_processor->send(getPlayersId(), "p_info", rfcArgParser::CreateObject(player.second->getPlayerInfo()));
                                Monster_death(monster);
                            } else if (monster.second->getType() == Entity::Monster::SHOTGUN_WEAPON) {
                                player.second->setWeapon(Entity::Player::SHOTGUN_SHOOT);
                                command_processor->send(getPlayersId(), "p_info", rfcArgParser::CreateObject(player.second->getPlayerInfo()));
                                Monster_death(monster);
                            } else if (monster.second->getType() == Entity::Monster::KAMIKAZE_MONSTER) {
                                if (player.second->getCollisionTimer().hasElapsed()) {
                                    player.second->TakeDamage(monster.second->getDamage());
                                    command_processor->send(getPlayersId(), "p_damage", std::to_string(player.second->getId()) + " " + std::to_string(monster.second->getDamage()));
                                    if (player.second->getHealth() <= 0)
                                        Player_death(player);
                                    Monster_death(monster);
                                    player.second->getCollisionTimer().reset();
                                }
                            } else if (monster.second->getType() == Entity::Monster::BOSS) {
                                if (player.second->getCollisionTimer().hasElapsed()) {
                                    player.second->TakeDamage(monster.second->getDamage());
                                    command_processor->send(getPlayersId(), "p_damage", std::to_string(player.second->getId()) + " " + std::to_string(monster.second->getDamage()));
                                    if (player.second->getHealth() <= 0)
                                        Player_death(player);
                                    player.second->getCollisionTimer().reset();
                                }
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
                auto shoots = monster.second->getShoots();
                if (!shoots.empty())
                    command_processor->send(getPlayersId(), "shoot", rfcArgParser::CreateObject(shoots.back()->getShootInfo()));
            }
            takeShoot(monster);
        }

        void Room::bossMonster(std::pair<int, std::shared_ptr<Entity::Monster>> monster)
        {
            if (monster.second->getHealth() <= 250) {
                monster.second->setPhase(3);
            } else if (monster.second->getHealth() <= 500) {
                monster.second->setPhase(2);
            } else {
                monster.second->setPhase(1);
            }
            if (monster.second->getPosX() <= -100) {
                monster.second->setPosX(700);
                monster.second->setRuee(false);
                monster.second->getRushTimer().reset();
            }
            if (monster.second->getPhase() >= 1) {
                if (monster.second->getPosY() >= 100 && monster.second->getPosY() <= 800) {
                    int random = rand() % 2;
                    if (random == 0)
                        monster.second->setPosY(monster.second->getPosY() - rand() % 10);
                    else
                        monster.second->setPosY(monster.second->getPosY() + rand() % 10);
                }
            }
            if (monster.second->getShootTimer().hasElapsed()) {
                monster.second->shoot();
                monster.second->getShootTimer().reset();
                auto shoots = monster.second->getShoots();
                if (!shoots.empty())
                    command_processor->send(getPlayersId(), "shoot", rfcArgParser::CreateObject(shoots.back()->getShootInfo()));
                takeShoot(monster);
            }
            if (monster.second->getPhase() >= 2) {
                if (monster.second->getRushTimer().hasElapsed() && monster.second->getRuee() == false) {
                    int newY = (std::rand() % 2 == 0) ? 200 : 400;
                    monster.second->setPosY(newY);
                    monster.second->setPosX(monster.second->getPosX() - 15);
                    monster.second->getRushTimer().reset();
                    monster.second->setRuee(true);
                }
            }
            if (monster.second->getPhase() == 3) {
                if (monster.second->getSpawnTimer().hasElapsed()) {
                    spawnMonster();
                    monster.second->getSpawnTimer().reset();
                }
            }
            if (monster.second->getRuee() == true)
                monster.second->setPosX(monster.second->getPosX() - 15);
            takeShoot(monster);
        }

        void Room::takeShoot(std::pair<int, std::shared_ptr<Entity::Monster>> monster)
        {
            auto shoots = monster.second->getShoots();
            for (auto &shoot : shoots) {
                if (!shoot->getIsActive()) {
                    command_processor->send(getPlayersId(), "s_death", rfcArgParser::CreateObject(shoot->getShootInfo()));
                    monster.second->removeShoot(shoot->getId());
                    continue;
                }

                shoot->update();
                for (auto player = _players.begin(); player != _players.end(); ++player) {
                    if (!player->second->getIsAlive())
                        continue;
                    if (checkCollision(shoot->getPosition(), 5, player->second->getPosition(), player->second->getSize())) {
                        player->second->TakeDamage(monster.second->getDamage());
                        command_processor->send(getPlayersId(), "s_death", rfcArgParser::CreateObject(shoot->getShootInfo()));
                        monster.second->removeShoot(shoot->getId());
                        command_processor->send(getPlayersId(), "p_damage", std::to_string(player->second->getId()) + " " + std::to_string(monster.second->getDamage()));
                        if (player->second->getHealth() <= 0)
                            Player_death(*player);
                    }
                }
            }
        }

        void Room::kamikazeMonster(std::pair<int, std::shared_ptr<Entity::Monster>> monster)
        {
        }

        void Room::bonusHandler(std::pair<int, std::shared_ptr<Entity::Monster>> monster)
        {
        }

        void Room::monstersUpdate()
        {
            if (_monsters.empty())
                return;
            int MonsterTypes[] =
                {Entity::Monster::BASIC_MONSTER, Entity::Monster::KAMIKAZE_MONSTER,
                 Entity::Monster::BOSS, Entity::Monster::HEALTH_BONUS,
                 Entity::Monster::DAMAGE_BONUS, Entity::Monster::SHOTGUN_WEAPON, -1};
            void (Room::*monsterUpdate[])(std::pair<int, std::shared_ptr<Entity::Monster>>) =
                {&Room::basicMonster, &Room::kamikazeMonster, &Room::bossMonster,
                 &Room::bonusHandler, &Room::bonusHandler, &Room::bonusHandler};

            for (int i = 0; MonsterTypes[i] != -1; i++) {
                for (auto &monster : _monsters) {
                    monster.second->update();
                    if (monster.second->getType() == MonsterTypes[i])
                        (this->*monsterUpdate[i])(monster);
                    std::unordered_map<std::string, std::string> tmp;
                    tmp["x"] = std::to_string(monster.second->getPosX());
                    tmp["y"] = std::to_string(monster.second->getPosY());
                    command_processor->send(getPlayersId(), "e_position", std::to_string(monster.second->getId()) + " " + rfcArgParser::CreateObject(tmp));
                    if (monster.second->getPosX() < -100) {
                        monster.second->setPosX(800);
                        monster.second->setPosY(std::rand() % 600);
                    }
                }
            }
        }

        void Room::shootsUpdate()
        {
            for (auto &player : _players) {
                auto shoots = player.second->getShoots();
                for (auto &shoot : shoots) {
                    if (!shoot->getIsActive()) {
                        command_processor->send(getPlayersId(), "s_death", rfcArgParser::CreateObject(shoot->getShootInfo()));
                        player.second->removeShoot(shoot->getId());
                        continue;
                    }

                    std::unordered_map<std::string, std::string> tmp = shoot->getShootInfo();
                    std::unordered_map<std::string, std::string> data = {
                        {"x", std::to_string(shoot->getPosition().x)},
                        {"y", std::to_string(shoot->getPosition().y)}};
                    std::string data_str = rfcArgParser::CreateObject(tmp) + " " + rfcArgParser::CreateObject(data);
                    command_processor->send(getPlayersId(), "s_position", data_str);
                }
            }
            for (auto &monster : _monsters) {
                auto shoots = monster.second->getShoots();
                for (auto &shoot : shoots) {
                    if (!shoot->getIsActive()) {
                        command_processor->send(getPlayersId(), "s_death", rfcArgParser::CreateObject(shoot->getShootInfo()));
                        monster.second->removeShoot(shoot->getId());
                        continue;
                    }

                    std::unordered_map<std::string, std::string> tmp = shoot->getShootInfo();
                    std::unordered_map<std::string, std::string> data = {
                        {"x", std::to_string(shoot->getPosition().x)},
                        {"y", std::to_string(shoot->getPosition().y)}};
                    std::string data_str = rfcArgParser::CreateObject(tmp) + " " + rfcArgParser::CreateObject(data);
                    command_processor->send(getPlayersId(), "s_position", data_str);
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
                if (_wave == MAX_WAVE) {
                    command_processor->send(getPlayersId(), "end", "win");
                    _mode.store(Mode::END);
                    _stateCondVar.notify_all();
                    return;
                }
                std::cout << "Wave " << _wave << " ended" << std::endl;
                if (!nextWave())
                    return;
                std::cout << "Wave " << _wave << " started" << std::endl;

                if (_wave < BOSS_WAVE) {
                    std::cout << "Spawning " << _wave << " monsters" << std::endl;
                    for (int i = 0; i <= _wave; i++)
                        spawnMonster();
                } else if (_wave == BOSS_WAVE) {
                    std::cout << "Spawning Boss" << std::endl;
                    spawnBoss();
                    return;
                }
                resetPlayers();
            }
            spawnBonusMonster();
            playersUpdate();
            monstersUpdate();
            shootsUpdate();
        }

        bool Room::checkCollision(const Game::Entity::Position &pos1, int size1, const Game::Entity::Position &pos2, int size2)
        {
            return (pos1.x < pos2.x + size2 &&
                    pos1.x + size1 > pos2.x &&
                    pos1.y < pos2.y + size2 &&
                    pos1.y + size1 > pos2.y);
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
                command_processor->send(getPlayersId(), "enemy", rfcArgParser::CreateObject(monster->getEnemyInfo()));
            } catch (const std::exception &e) {
                std::cerr << "Error spawning monster: " << e.what() << std::endl;
                return;
            }
        }

        void Room::spawnBoss()
        {
            std::cout << "Spawning Boss" << std::endl;
            try {
                int monsterId = _monsters.size() + 1;
                std::cout << "Monster " << monsterId << " spawned" << std::endl;
                auto monster = std::make_shared<Game::Entity::Monster>(monsterId, _wave);
                std::cout << "Monster " << monsterId << " spawned" << std::endl;
                monster->setType(RType::Game::Entity::Monster::BOSS);
                monster->setHealth(1000);
                monster->setDamage(25);
                monster->setPosY(300);
                monster->setPosX(700);
                monster->setPhase(1);
                monster->setSize(100);

                std::cout << "Monster " << monsterId << " spawned at " << monster->getPosX() << ", " << monster->getPosY()
                          << "Type of " << monster->getType() << std::endl;
                std::lock_guard<std::mutex> lock(_monsterMutex);
                _monsters[monsterId] = monster;
                command_processor->send(getPlayersId(), "enemy", rfcArgParser::CreateObject(monster->getEnemyInfo()));
            } catch (const std::exception &e) {
                std::cerr << "Error spawning monster: " << e.what() << std::endl;
                return;
            }
        }

        void Room::spawnBonusMonster()
        {
            try {
                if (_bonusSpawn.hasElapsed() == false)
                    return;
                _bonusSpawn.reset();
                int monsterId = _monsters.size() + 1;
                auto monster = std::make_shared<Game::Entity::Monster>(monsterId, _wave);

                std::vector<std::pair<RType::Game::Entity::Monster::Type, std::string>> bonusTypes = {
                    {RType::Game::Entity::Monster::HEALTH_BONUS, "bonus_health"},
                    {RType::Game::Entity::Monster::DAMAGE_BONUS, "bonus_damage"},
                    {RType::Game::Entity::Monster::SHOTGUN_WEAPON, "weapon_shotgun"}};

                auto selectedBonus = bonusTypes[std::rand() % bonusTypes.size()];
                monster->setType(selectedBonus.first);

                monster->setHealth(1);
                monster->setDamage(0);
                monster->setPosX(800);
                monster->setPosY(std::rand() % 500 + 50);

                std::cout << "Bonus " << monsterId << " spawned at " << monster->getPosX() << ", " << monster->getPosY()
                          << " Type: " << monster->getType() << std::endl;

                std::lock_guard<std::mutex> lock(_monsterMutex);
                _monsters[monsterId] = monster;
                command_processor->send(getPlayersId(), "enemy", rfcArgParser::CreateObject(monster->getEnemyInfo()));

            } catch (const std::exception &e) {
                std::cerr << "Error spawning bonus monster: " << e.what() << std::endl;
                return;
            }
        }

        bool Room::addPlayer(std::shared_ptr<Game::Entity::Player> player)
        {
            std::lock_guard<std::mutex> lock(_playerMutex);
            if (_wave > 1) return false;
            if ((int)_players.size() > MAX_PLAYER) return false;
            _players[player->getId()] = player;
            return true;
        }

        void Room::removePlayer(int playerId)
        {
            std::lock_guard<std::mutex> lock(_playerMutex);
            _players.erase(playerId);
        }

        Room::~Room()
        {
            stop();
        }

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

        Timer &Room::getMonsterUpdate()
        {
            return _monsterTimer;
        }

        Timer &Room::getSpawnBonus()
        {
            return _bonusSpawn;
        }

        bool Room::isRunning() const
        {
            return _isReady.load() && !_shouldStop.load();
        }

        std::unordered_map<std::string, std::string> Room::getRoomInfo() const
        {
            std::unordered_map<std::string, std::string> data;
            data["id"] = std::to_string(_id);
            data["name"] = _name;
            data["count"] = std::to_string(_players.size());
            data["mode"] = "Quick-Play";
            return data;
        }

        std::unordered_map<int, std::shared_ptr<Game::Entity::Player>> &Room::getPlayers()
        {
            return _players;
        }
    } // namespace Game
} // namespace RType
