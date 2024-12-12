/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Room class
** Caroline Boilly @Aeliondw
*/

#include <Room.hpp>

namespace Server
{
    Room::Room(int id, std::string name)
    {
        _id = id;
        _name = name;
        _lastMonsterSpawn = std::chrono::steady_clock::now();

        std::cout << "Room " << _id << " initiated" << std::endl;
    }

    Room::~Room()
    {
        std::cout << "Room " << _id << " destroyed" << std::endl;
    }

    /*  ---- GAME LOGIC ---- */
//TODO: Add Exception Class
    void Room::addPlayer(const Player &player)
    {
        if (_count >= MAX_PLAYER) {
            std::cerr << "Room " << _id << " is full. Cannot add player." << std::endl;
            return;
        }
        _players.push_back(player);
        _count++;
        std::cout << "Player " << player.getId() << " joined Room " << _id << std::endl;
    }

    void Room::removePlayer(int playerId)
    {
        _players.erase(std::remove_if(_players.begin(), _players.end(),
                                    [playerId](const Player &player) { return player.getId() == playerId; }),
                    _players.end());
        _count--;
        std::cout << "Player " << playerId << " left Room " << _id << std::endl;
    }

//TODO: Set monster level with room level
//TODO: Set pos_x with screen width
    void Room::spawnMonster()
    {
        int monsterId = _monsters.size() + 1;
        int level = 1;
        Monster monster(monsterId, level);
        monster.setPosX(1920);
        monster.setPosY(std::rand() % 1080);
        _monsters.push_back(monster);
        std::cout << "Monster " << monster.getId() << " spawned." << std::endl;
    }

//TODO: Monster spawn all 5 seconds (change to random after mvp)
//TODO: Update Game Mode with Mode::PLAYING 
//TODO: Lose condition (when all players.isAlive == false)
void Room::update()
{
    auto now = std::chrono::steady_clock::now();
    if (_mode != Mode::PLAYING)
        return;

    if (std::chrono::duration_cast<std::chrono::seconds>(now - _lastMonsterSpawn).count() >= 5) {
        spawnMonster();
        _lastMonsterSpawn = now;
    }

    for (auto &player : _players) {
        if (player.getIsAlive()) {
            player.update();

            for (auto &shoot : player.getShoots()) {
                shoot.updatePosition();

                for (auto &monster : _monsters) {
                    if (shoot.getPosX() == monster.getPosX() && shoot.getPosY() == monster.getPosY()) {
                        monster.setHealth(monster.getHealth() - shoot.getDamage());
                        shoot.setIsActive(false);
                        if (monster.getHealth() <= 0) {
                            monster.setIsAlive(false);
                            std::cout << "Monster " << monster.getId() << " was killed by player " << player.getId() << std::endl;
                        }
                    }
                }
            }
            player.removeInactiveShoots();
        }
    }

    for (auto &monster : _monsters) {
        monster.update();

        for (auto &shoot : monster.getShoots()) {
            shoot.updatePosition();

            for (auto &player : _players) {
                if (shoot.getPosX() == player.getPosX() && shoot.getPosY() == player.getPosY()) {
                    player.setHealth(player.getHealth() - shoot.getDamage());
                    shoot.setIsActive(false);
                    if (player.getHealth() <= 0) {
                        player.setIsAlive(false);
                        std::cout << "Player " << player.getId() << " was killed by monster " << monster.getId() << ".\n";
                    }
                }
            }
        }
        monster.removeInactiveShoots();
    }

    _monsters.erase(std::remove_if(_monsters.begin(), _monsters.end(),
                                    [](const Monster &monster) { return !monster.getIsAlive(); }),
                                    _monsters.end());

    _players.erase(std::remove_if(_players.begin(), _players.end(),
                                    [](const Player &player) { return !player.getHaveJoined(); }),
                                    _players.end());
}

    /*  ---- SETTER ---- */

    void Room::setName(std::string name)
    { 
        _name = name;
    }

    void Room::setMode(Mode mode)
    { 
        _mode = mode;
    }


    /*  ---- GETTER ---- */

    std::unordered_map<std::string, std::string> Room::getRoomInfo() const
    {
        return {
            {"id", std::to_string(_id)},
            {"name", _name},
            {"count", std::to_string(_count)},
            {"mode", std::to_string(static_cast<int>(_mode))}};
        };
    }

    std::string Room::getName() const
    { 
        return _name;
    }

    int Room::getCount() const
    { 
        return _count;
    }

    int Room::getMode() const
    { 
        return _mode;
    }

} // namespace Server
