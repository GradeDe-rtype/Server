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
                command_processor->send(-1, "end", "lost");
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
                command_processor->send(-1, "wave", std::to_string(_wave));
            }

            for (auto &player : _players) {
                if (player.second->getHaveJoined() == false) {
                    command_processor->to_send(player.second->getId(), "wave", std::to_string(_wave));
                    player.second->setWeapon(RType::Game::Entity::Player::Shoot_Type::BASIC_SHOOT);
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
                player.second->setWeapon(Entity::Player::Shoot_Type::BASIC_SHOOT);
            }
        }

        void Room::Monster_death(const std::pair<int, std::shared_ptr<Entity::Monster>> &it)
        {
            if (it.second->getType() == RType::Game::Entity::Monster::Type::BOSS) {
                auto Bshoots = it.second->getShoots();
                for (auto &Bshoot : Bshoots) {
                    Bshoot->setIsActive(false);
                    std::unordered_map<std::string, std::string> mess = Bshoot->getShootInfo();
                    command_processor->send(-1, "s_death", rfcArgParser::CreateObject(mess));
                    it.second->removeShoot(Bshoot->getId());
                }
                command_processor->send(-1, "e_death", std::to_string(it.second->getId()));
                _monsters.erase(it.first);
                for (auto &monster : _monsters)
                    Monster_death(monster);
                return;
            }
            command_processor->send(-1, "e_death", std::to_string(it.second->getId()));
            auto Mshoots = it.second->getShoots();
            for (auto &Mshoot : Mshoots) {
                Mshoot->setIsActive(false);
                std::unordered_map<std::string, std::string> mess = Mshoot->getShootInfo();
                command_processor->send(-1, "s_death", rfcArgParser::CreateObject(mess));
                it.second->removeShoot(Mshoot->getId());
            }
            _monsters.erase(it.first);
        }

        void Room::Player_death(const std::pair<int, std::shared_ptr<Entity::Player>> &it)
        {
            command_processor->send(-1, "p_death", std::to_string(it.second->getId()));
            it.second->setIsAlive(false);
            auto Pshoots = it.second->getShoots();
            for (auto &Pshoot : Pshoots) {
                Pshoot->setIsActive(false);
                std::unordered_map<std::string, std::string> mess = Pshoot->getShootInfo();
                command_processor->send(-1, "s_death", rfcArgParser::CreateObject(mess));
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
                                command_processor->send(-1, "s_death", rfcArgParser::CreateObject(shoot->getShootInfo()));
                                player.second->removeShoot(shoot->getId());
                                command_processor->send(-1, "e_damage", std::to_string(it.second->getId()) + " " + std::to_string(player.second->getDamage()));
                                if (it.second->getHealth() <= 0)
                                    Monster_death(it);
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
                    command_processor->send(-1, "shoot", rfcArgParser::CreateObject(shoots.back()->getShootInfo()));
            }
            takeShoot(monster);
        }

        void Room::bossMonster(std::pair<int, std::shared_ptr<Entity::Monster>> monster)
        {
            if (monster.second->getPosX() <= -100) {
                monster.second->setPosX(800);
                monster.second->setRuee(false);
                monster.second->getRushTimer().reset();
            }
            if (monster.second->getPosX() > 750) {
                monster.second->setPosX(monster.second->getPosX() - 3);
                return;
            }

            if (monster.second->getPosX() <= 750) {
                if (monster.second->getHealth() > 500 && monster.second->getPhase() == 1) {
                    if (monster.second->getShootTimer().hasElapsed()) {
                        monster.second->shoot();
                        monster.second->getShootTimer().reset();
                        auto shoots = monster.second->getShoots();
                        if (!shoots.empty())
                            command_processor->send(-1, "shoot", rfcArgParser::CreateObject(shoots.back()->getShootInfo()));
                    }
                    if (monster.second->getSpawnTimer().hasElapsed()) {
                        spawnMonster();
                        monster.second->getSpawnTimer().reset();
                    }
                } else if (monster.second->getHealth() > 250 && monster.second->getPhase() == 2) {
                    if (monster.second->getShootTimer().hasElapsed()) {
                        monster.second->shoot();
                        monster.second->getShootTimer().reset();
                        auto shoots = monster.second->getShoots();
                        if (!shoots.empty())
                            command_processor->send(-1, "shoot", rfcArgParser::CreateObject(shoots.back()->getShootInfo()));
                    }
                    if (monster.second->getRushTimer().hasElapsed() && monster.second->getRuee() == false) {
                        int newY = (std::rand() % 2 == 0) ? 200 : 400;
                        monster.second->setPosY(newY);
                        monster.second->setPosX(monster.second->getPosX() - 15);
                        monster.second->getRushTimer().reset();
                        monster.second->setRuee(true);
                    }
                } else {
                    if (monster.second->getRushTimer().hasElapsed() && monster.second->getRuee() == false) {
                        int newY = (std::rand() % 2 == 0) ? 200 : 400;
                        monster.second->setPosY(newY);
                        monster.second->setPosX(monster.second->getPosX() - 15);
                        monster.second->getRushTimer().reset();
                        monster.second->setRuee(true);
                    } else {
                        if (monster.second->getShootTimer().hasElapsed()) {
                            monster.second->shoot();
                            monster.second->getShootTimer().reset();
                            auto shoots = monster.second->getShoots();
                            if (!shoots.empty())
                                command_processor->send(-1, "shoot", rfcArgParser::CreateObject(shoots.back()->getShootInfo()));
                        }
                        if (monster.second->getSpawnTimer().hasElapsed()) {
                            spawnMonster();
                            monster.second->getSpawnTimer().reset();
                        }
                    }
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
                    monster.second->removeShoot(shoot->getId());
                    continue;
                }

                shoot->update();
                for (auto player = _players.begin(); player != _players.end(); ++player) {
                    if (!player->second->getIsAlive())
                        continue;
                    if (checkCollision(shoot->getPosition(), 1, player->second->getPosition(), player->second->getSize())) {
                        player->second->TakeDamage(monster.second->getDamage());
                        command_processor->send(-1, "s_death", rfcArgParser::CreateObject(shoot->getShootInfo()));
                        monster.second->removeShoot(shoot->getId());
                        command_processor->send(-1, "p_damage", std::to_string(player->second->getId()) + " " + std::to_string(monster.second->getDamage()));
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
            if (monster.second->getPosX() <= -100) {
                monster.second->setIsAlive(false);
                monster.second->setHealth(0);
                Monster_death(monster);
            }
        }

        void Room::monstersUpdate()
        {
            int MonsterTypes[] =
                {Entity::Monster::BASIC_MONSTER,
                Entity::Monster::KAMIKAZE_MONSTER,
                Entity::Monster::BOSS,
                Entity::Monster::HEALTH_BONUS,
                Entity::Monster::DAMAGE_BONUS,
                Entity::Monster::SHOTGUN_WEAPON, -1};
            void (Room::*monsterUpdate[])(std::pair<int, std::shared_ptr<Entity::Monster>>) =
                {&Room::basicMonster,
                &Room::kamikazeMonster,
                &Room::bossMonster,
                &Room::bonusHandler,
                &Room::bonusHandler,
                &Room::bonusHandler };

            for (int i = 0; MonsterTypes[i] != -1; i++) {
                for (auto &monster : _monsters) {
                    monster.second->update();
                    if (monster.second->getType() == MonsterTypes[i]) {
                        (this->*monsterUpdate[i])(monster);
                    }

                    std::unordered_map<std::string, std::string> tmp;
                    tmp["x"] = std::to_string(monster.second->getPosX());
                    tmp["y"] = std::to_string(monster.second->getPosY());
                    command_processor->send(-1, "e_position", std::to_string(monster.second->getId()) + " " + rfcArgParser::CreateObject(tmp));
                }
            }

            for (auto &_monster : _monsters) {
                if (_monster.second->getPosX() < -100) {
                    _monster.second->setPosX(800);
                    _monster.second->setPosY(std::rand() % 600);
                }
            }
        }

        void Room::shootsUpdate()
        {
            for (auto &player : _players) {
                auto shoots = player.second->getShoots();
                for (auto &shoot : shoots) {
                    if (!shoot->getIsActive()) {
                        command_processor->send(-1, "s_death", rfcArgParser::CreateObject(shoot->getShootInfo()));
                        player.second->removeShoot(shoot->getId());
                        continue;
                    }

                    std::unordered_map<std::string, std::string> tmp = shoot->getShootInfo();
                    std::unordered_map<std::string, std::string> data = {
                        {"x", std::to_string(shoot->getPosition().x)},
                        {"y", std::to_string(shoot->getPosition().y)}};
                    std::string data_str = rfcArgParser::CreateObject(tmp) + " " + rfcArgParser::CreateObject(data);
                    command_processor->send(-1, "s_position", data_str);
                }
            }
            for (auto &monster : _monsters) {
                auto shoots = monster.second->getShoots();
                for (auto &shoot : shoots) {
                    if (!shoot->getIsActive()) {
                        command_processor->send(-1, "s_death", rfcArgParser::CreateObject(shoot->getShootInfo()));
                        monster.second->removeShoot(shoot->getId());
                        continue;
                    }

                    std::unordered_map<std::string, std::string> tmp = shoot->getShootInfo();
                    std::unordered_map<std::string, std::string> data = {
                        {"x", std::to_string(shoot->getPosition().x)},
                        {"y", std::to_string(shoot->getPosition().y)}};
                    std::string data_str = rfcArgParser::CreateObject(tmp) + " " + rfcArgParser::CreateObject(data);
                    command_processor->send(-1, "s_position", data_str);
                }
            }
        }

        void Room::bossUpdate()
        {
            if (_wave == BOSS_WAVE) {
                if (_monsters.begin()->second->getType() == RType::Game::Entity::Monster::BOSS) {
                    if (_monsters.begin()->second->getHealth() <= 500) {
                        _monsters.begin()->second->setPhase(2);
                        std::cout << "Boss phase 2" << std::endl;
                    } else if (_monsters.begin()->second->getHealth() <= 250) {
                        _monsters.begin()->second->setPhase(3);
                        std::cout << "Boss phase 3" << std::endl;
                    }
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

                if (_wave < BOSS_WAVE) {
                    std::cout << "Spawning " << _wave << " monsters" << std::endl;
                    for (int i = 0; i <= _wave; i++)
                        spawnMonster();
                } else if (_wave == BOSS_WAVE) {
                    std::cout << "Spawning Boss" << std::endl;
                    spawnBoss();
                    return;
                } else {
                    command_processor->send(-1, "end", "win");
                    _mode.store(Mode::END);
                    return;
                }
                resetPlayers();
            }
            playersUpdate();
            monstersUpdate();
            shootsUpdate();
            bossUpdate();
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
                command_processor->send(-1, "enemy", rfcArgParser::CreateObject(monster->getEnemyInfo()));
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
                monster->setPosX(600);
                monster->setPhase(1);
                monster->setSize(100);

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

        void Room::spawnBonusMonster()
        {
            try {
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
                command_processor->send(-1, "enemy", rfcArgParser::CreateObject(monster->getEnemyInfo()));

            } catch (const std::exception &e) {
                std::cerr << "Error spawning bonus monster: " << e.what() << std::endl;
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
