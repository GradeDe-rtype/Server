/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen
    >> Daniil Stepanov @dan13615

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include "Room.hpp"

//TODO: Send _position players to all connected player after a delay
//TODO: Send damage a player received by a monster to all player
//TODO: Send that a player die to all player

//TODO: Send the position of a player shoot to all player
//TODO: Send enemy-info to all player when a new enemy has been created

//TODO: Send _position enemy to all connected player after a delay
//TODO: Send damage a monster received by a player to all player
//TODO: Send that a monster die to all player
//TODO: Send a request to all player if they are ready to a new wave


namespace Server
{
    Room::Room(int id, std::string name) : _monsterSpawnTimer(5000)
    {
        _id = id;
        _name = name;

        std::cout << "Room " << _id << " initiated" << std::endl;
    }

    Room::~Room()
    {
        std::cout << "Room " << _id << " destroyed" << std::endl;
    }

    /*  ---- GAME LOGIC ---- */
    void Room::start()
    {
        _mode = Mode::PLAYING;
    }

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

//TODO: Recheck monsterId
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

//TODO AFTER MVP: Monster spawn all 5 seconds (change to random)
//TODO: Lose condition (when all players.isAlive == false)
void Room::update()
    {
        if (!_isReady && _mode != Mode::PLAYING)
            return;

        auto now = std::chrono::steady_clock::now();
        if (_monsterSpawnTimer.hasElapsed()) {
            spawnMonster();
            _monsterSpawnTimer.reset();
        }

        // TODO RFC: Send enemy-info to all players when a new enemy has been created
        // for (const auto &monster : _monsters) {
        //     sendEnemyInfoToAllPlayers(monster);
        // }

        for (auto &player : _players) {
            if (player.getIsAlive()) {
                player.update();

                for (auto &shoot : player.getShoots()) {
                    shoot.updatePosition();

                    for (auto &monster : _monsters) {
                        //TODO: Get range
                        if (shoot.getPosX() == monster.getPosX() && shoot.getPosY() == monster.getPosY()) {
                            monster.setHealth(monster.getHealth() - shoot.getDamage());
                            shoot.setIsActive(false);

                            // TODO RFC: Send damage a monster received by a player to all players
                            // sendMonsterDamageToAllPlayers(player.getId(), monster.getId(), shoot.getDamage());

                            if (monster.getHealth() <= 0) {
                                monster.setIsAlive(false);
                                
                                // TODO RFC: Send that a monster died to all players
                                // sendMonsterDeathToAllPlayers(monster);
                                
                                std::cout << "Monster " << monster.getId() << " was killed by player " << player.getId() << std::endl;
                            }
                        }
                    }
                }
                player.removeInactiveShoots();
            }

            // TODO RFC: Send the position of a player shoot to all players
            // for (const auto &shoot : player.getShoots()) {
            //      sendPlayerShootPositionToAllPlayers(player.getId(), shoot);
            // }
        }

        for (auto &monster : _monsters) {
            monster.update();

            for (auto &shoot : monster.getShoots()) {
                shoot.updatePosition();

                for (auto &player : _players) {
                    if (shoot.getPosX() == player.getPosX() && shoot.getPosY() == player.getPosY()) {
                        player.setHealth(player.getHealth() - shoot.getDamage());
                        shoot.setIsActive(false);

                        // TODO RFC: Send damage a player received by a monster to all players
                        // sendPlayerDamageToAllPlayers(monster.getId(), player.getId(), shoot.getDamage());

                        if (player.getHealth() <= 0) {
                            player.setIsAlive(false);

                            // TODO RFC: Send that a player died to all players
                            // sendPlayerDeathToAllPlayers(player);

                            std::cout << "Player " << player.getId() << " was killed by monster " << monster.getId() << ".\n";
                        }
                    }
                }
            }
            monster.removeInactiveShoots();
        }

        // TODO RFC: Send position of all players to all connected players after a delay
        // for (const auto &player : _players) {
        //     sendPlayerPositionToAllPlayers(player);
        // }

        // TODO RFC: Send position of all enemies to all connected players after a delay
        // for (const auto &monster : _monsters) {
        //     sendEnemyPositionToAllPlayers(monster);
        // }

        // TODO RFC: Send a request to all players if they are ready for a new wave
        // if (checkIfWaveReady()) {
        //     sendWaveReadyRequestToAllPlayers();
        // }

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

//TODO: doc
    void Room::setIsReady(bool isReady)
    { 
        _isReady = isReady;
    }

    void Room::setScore(int score)
    {
        _score = score;
    }


    /*  ---- GETTER ---- */

    std::unordered_map<std::string, std::string> Room::getRoomInfo() const
    {
        return {
            {"id", std::to_string(_id)},
            {"name", _name},
            {"count", std::to_string(_count)},
            {"mode", std::to_string(static_cast<int>(_mode))}
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

    Room::Mode Room::getMode() const
    { 
        return _mode;
    }

//TODO: doc
    bool Room::getIsReady() const
    { 
        return _isReady;
    }

//TODO: Send the score to all players at end of the game
    int Room::getScore() const
    {
        return _score;
    }

} // namespace Server
