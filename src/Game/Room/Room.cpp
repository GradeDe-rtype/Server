/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen
    >> Daniil Stepanov @dan13615

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include "Room.hpp"

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
        Room::Room(int id, std::string name)
            : _monsterSpawnTimer(5000)
        {
            _id = id;
            _name = name;
        }

        /*  ---- GAME LOGIC ---- */
        void Room::start()
        {
            _mode = Mode::PLAYING;
        }

        void Room::addPlayer(std::shared_ptr<Game::Entity::Player> &player)
        {
            if ((int)_players.size() >= MAX_PLAYER) return;
            _players[player->getId()] = player;
        }

        void Room::removePlayer(int playerId)
        {
            _players.erase(playerId);
        }

        void Room::spawnMonster()
        {
            int monsterId = _monsters.size() + 1;
            int level = 1;
            std::shared_ptr<Game::Entity::Monster> monster = std::make_shared<Game::Entity::Monster>(monsterId, level);
            monster->setPosX(1920);
            monster->setPosY(std::rand() % 1080);
            _monsters[monsterId] = monster;
            // TODO RFC: Send `enemy` with all the info to all the players
        }

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
                            Game::Entity::Position shootPos = shoot->getPosition();
                            Game::Entity::Position monsterPos = it->second->getPosition();
                            int monsterSize = it->second->getSize();
                            if (shootPos.x >= monsterPos.x && shootPos.x <= monsterPos.x + monsterSize && shootPos.y >= monsterPos.y && shootPos.y <= monsterPos.y + monsterSize) {
                                // TODO RFC: Send `e_death` to all players
                                it = _monsters.erase(it);
                            } else
                                ++it;
                        }
                    }
                    player.second->update();
                    // TODO RFC: Send `p_position` to all players if player move
                }
            }

            for (auto &monster : _monsters) {
                for (auto &shoot : monster.second->getShoots()) {
                    shoot->update();
                    for (auto it = _players.begin(); it != _players.end();) {
                        Game::Entity::Position shootPos = shoot->getPosition();
                        Game::Entity::Position playerPos = it->second->getPosition();
                        int playerSize = it->second->getSize();
                        if (shootPos.x >= playerPos.x && shootPos.x <= playerPos.x + playerSize && shootPos.y >= playerPos.y && shootPos.y <= playerPos.y + playerSize) {
                            // TODO RFC: Send `p_death` to all players
                            it = _players.erase(it);
                        } else
                            ++it;
                    }
                }
                monster.second->update();
                // Maybe move the enemy
                // TODO RFC: Send `e_position` to all players if enemy move
            }

            // TODO RFC: Send a request to all players if they are ready for a new wave
            // if (checkIfWaveReady()) {
            //     sendWaveReadyRequestToAllPlayers();
            // }
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
                {"count", std::to_string((int)_players.size())},
                {"mode", std::to_string(static_cast<int>(_mode))}};
        }

        std::string Room::getName() const
        {
            return _name;
        }

        int Room::getCount() const
        {
            return (int)_players.size();
        }

        Room::Mode Room::getMode() const
        {
            return _mode;
        }

        bool Room::getIsReady() const
        {
            return _isReady;
        }

        int Room::getScore() const
        {
            return _score;
        }
    } // namespace Game
} // namespace RType
