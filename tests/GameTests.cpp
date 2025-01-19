#include <boost/asio.hpp>
#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <criterion/redirect.h>
#include <memory>

#include "Colors.hpp"
#include "Command.hpp"
#include "Monster.hpp"
#include "Player.hpp"
#include "Room.hpp"
#include "Shoot.hpp"
#include "Timer.hpp"

namespace RType
{
    namespace Game
    {

        /* ---- COLORS CLASS ---- */

        Test(Colors, GetValidColors)
        {
            Colors &colors = Colors::get();

            cr_assert_eq(colors.getColor(0), "#FFd0000", "The color at index 0 should be #FF0000");
            cr_assert_eq(colors.getColor(1), "#00FF00", "The color at index 1 should be #00FF00");
            cr_assert_eq(colors.getColor(2), "#0000FF", "The color at index 2 should be #0000FF");
            cr_assert_eq(colors.getColor(3), "#FFFF00", "The color at index 3 should be #FFFF00");
        }

        Test(Colors, GetColorOutOfBounds)
        {
            Colors &colors = Colors::get();

            cr_assert_eq(colors.getColor(8), "#FF0000", "Index 8 should loop and return the color of index 0 (#FF0000)");
            cr_assert_eq(colors.getColor(9), "#00FF00", "Index 9 should loop and return the color of index 1 (#00FF00)");
        }

        Test(Colors, SingletonInstance)
        {
            Colors &instance1 = Colors::get();
            Colors &instance2 = Colors::get();

            cr_assert_eq(&instance1, &instance2, "Both instances should point to the same Colors object");
        }

        Test(Colors, LargeIndexTest)
        {
            Colors &colors = Colors::get();

            cr_assert_eq(colors.getColor(15), "#800080", "Index 15 should loop properly (15 % 8 = 7 -> #800080)");
            cr_assert_eq(colors.getColor(20), "#FF00FF", "Index 20 should loop properly (20 % 8 = 4 -> #FF00FF)");
        }

        /* ---- TIMER TESTS ---- */

        Test(Timer, TimerInitialization)
        {
            Timer timer(1000);
            cr_assert(timer.timeLeft() <= 1000, "The timer should have at most 1000ms left at initialization.");
        }

        Test(Timer, TimerReset)
        {
            Timer timer(500);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            timer.reset();
            cr_assert(timer.timeLeft() >= 400, "After reset, the timer should have close to 500ms left.");
        }

        Test(Timer, TimerHasElapsed)
        {
            Timer timer(200);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            cr_assert(timer.hasElapsed(), "Timer should have elapsed after 300ms.");
        }

        /* ---- ROOM TESTS ---- */

        Test(Room, RoomCreation)
        {
            boost::asio::io_context io_context;
            Server::GameServer gameServer(io_context, 0); // Port dynamique
            std::unique_ptr<Server::Command> dummy_command = std::make_unique<Server::Command>(gameServer);
            auto room = RType::Game::Room::create(1, "RoomCreation", dummy_command.get());

            cr_assert(room != nullptr, "Room should be created successfully.");
            cr_assert_eq(room->getID(), 1, "Room ID should be 1.");
            cr_assert_eq(room->getName(), "RoomCreation", "Room name should match the given name.");

            room->stop();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        Test(Room, AddRemovePlayer)
        {
            boost::asio::io_context io_context;
            Server::GameServer gameServer(io_context, 0);
            std::unique_ptr<Server::Command> dummy_command = std::make_unique<Server::Command>(gameServer);
            auto room = RType::Game::Room::create(2, "AddRemovePlayer", dummy_command.get());

            auto socket = std::make_shared<boost::asio::ip::tcp::socket>(boost::asio::make_strand(io_context));
            auto player = std::make_shared<Game::Entity::Player>(1, socket);

            cr_assert(room->addPlayer(player), "Adding a player should return true.");
            cr_assert_eq(room->getPlayers().size(), 1, "There should be 1 player in the room.");

            room->removePlayer(1);
            cr_assert_eq(room->getPlayers().size(), 0, "There should be no players in the room after removal.");

            room->stop();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        Test(Room, NextWave)
        {
            boost::asio::io_context io_context;
            Server::GameServer gameServer(io_context, 0);
            std::unique_ptr<Server::Command> dummy_command = std::make_unique<Server::Command>(gameServer);
            auto room = RType::Game::Room::create(4, "NextWave", dummy_command.get());

            cr_assert(room->nextWave(), "First call to nextWave() should return true.");

            room->stop();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        Test(Room, CheckCollision)
        {
            Server::Command *dummy_command = nullptr;
            auto room = Room::create(5, "CheckCollision", dummy_command);

            Game::Entity::Position pos1 = {10, 10};
            Game::Entity::Position pos2 = {15, 15};

            cr_assert(room->checkCollision(pos1, 10, pos2, 10), "Collision should be detected.");

            Game::Entity::Position pos3 = {100, 100};
            cr_assert(!room->checkCollision(pos1, 10, pos3, 10), "No collision should be detected.");
        }

        namespace Entity
        {

            /* ---- MONSTER CLASS ---- */

            Test(Monster, ConstructorDefault)
            {
                Monster monster(1);

                cr_assert_eq(monster.getId(), 1, "The monster ID should be 1.");
                cr_assert_eq(monster.getLevel(), 1, "The default level should be 1.");
                cr_assert_eq(monster.getHealth(), 25, "The default health should be 100.");
                cr_assert_eq(monster.getDamage(), 10, "Default damage should be 10.");
                cr_assert_eq(monster.getSpeed(), 8, "The monster speed should be 8.");
                cr_assert_eq(monster.getSize(), 40, "The monster size should be 8.");
                cr_assert(monster.getIsAlive(), "The monster should be alive by default.");
                cr_assert_eq(monster.getDirection(), Direction::LEFT, "The default direction should be LEFT.");
            }

            Test(Monster, ConstructorWithLevel)
            {
                Monster monster(2, 3);

                cr_assert_eq(monster.getId(), 2, "Monster ID should be 2.");
                cr_assert_eq(monster.getLevel(), 3, "Monster level should be 3.");
                cr_assert_eq(monster.getHealth(), 75, "The default health should be 25 * level.");
                cr_assert_eq(monster.getDamage(), 30, "Default damage should be 10 * level.");
                cr_assert_eq(monster.getSpeed(), 8, "The monster speed should be 8.");
                cr_assert_eq(monster.getSize(), 40, "The monster size should be 8.");
                cr_assert(monster.getIsAlive(), "The monster should be alive by default.");
                cr_assert_eq(monster.getDirection(), Direction::LEFT, "The default direction should be LEFT.");
            }

            Test(Monster, SetGetType)
            {
                Monster monster(3);
                monster.setType(Monster::Type::BOSS);

                cr_assert_eq(monster.getType(), Monster::Type::BOSS, "The monster type should be BOSS.");
            }

            Test(Monster, Shoot)
            {
                Monster monster(4);
                monster.shoot();

                auto shoots = monster.getShoots();
                cr_assert_eq(shoots.size(), 1, "There should be a shot after shoot().");
                cr_assert_eq(shoots[0]->getDamage(), monster.getDamage(), "Shot damage should match monster damage.");
            }

            Test(Monster, GetEnemyInfo)
            {
                Monster monster(6);
                auto info = monster.getEnemyInfo();

                cr_assert_eq(info["id"], "6", "The ID in EnemyInfo should be 6.");
                cr_assert_eq(info["type"], "0", "The default type should be MONSTER.");
                cr_assert_eq(info["size"], "40", "Size should be 40.");
                cr_assert_eq(info["health"], "25", "Health should be 25.");
            }

            /* ---- PLAYER CLASS ---- */

            Test(Player, ConstructorDefault)
            {
                boost::asio::io_context io_context;
                auto socket = std::make_shared<boost::asio::ip::tcp::socket>(boost::asio::make_strand(io_context));

                Player player(1, socket);

                cr_assert_eq(player.getId(), 1, "The constructor does not initialize the ID correctly.");
                cr_assert_eq(player.getLevel(), 1, "The constructor does not initialize the level correctly.");
                cr_assert_eq(player.getHealth(), 100, "Initial health must be 100.");
                cr_assert_eq(player.getDamage(), 25, "Initial damage must be 10.");
                cr_assert_eq(player.getSize(), 40, "Initial size must be 40.");
                cr_assert_eq(player.getSpeed(), 1, "Initial speed must be 1.");
                cr_assert(player.getIsAlive(), "Player must be alive after creation.");
                cr_assert_eq(player.getDirection(), Direction::RIGHT, "Initial direction must be RIGHT.");
            }

            Test(Player, ShootFunctionality)
            {
                boost::asio::io_context io_context;
                auto socket = std::make_shared<boost::asio::ip::tcp::socket>(boost::asio::make_strand(io_context));
                Player player(1, socket);

                player.shoot(10, 20);
                auto shoots = player.getShoots();

                cr_assert_eq(shoots.size(), 1, "A shot must be added after the call to shoot.");
                cr_assert_eq(shoots[0]->getPosX(), 10, "The position X of the shot is incorrect.");
                cr_assert_eq(shoots[0]->getPosY(), 20, "The Y position of the shot is incorrect.");
                cr_assert_eq(shoots[0]->getDamage(), player.getDamage(), "Shot damage must match player damage.");
            }

            Test(Player, SettersAndGetters)
            {
                boost::asio::io_context io_context;
                auto socket = std::make_shared<boost::asio::ip::tcp::socket>(boost::asio::make_strand(io_context));
                Player player(1, socket);

                player.setColor("#FFFFFF");
                player.setHaveJoined(true);

                cr_assert_eq(player.getColor(), "#FFFFFF", "Player color must be set correctly.");
                cr_assert(player.getHaveJoined(), "The ‘haveJoined’ state must be correctly set.");
            }

            Test(Player, GetPlayerInfo)
            {
                boost::asio::io_context io_context;
                auto socket = std::make_shared<boost::asio::ip::tcp::socket>(boost::asio::make_strand(io_context));
                Player player(1, socket);

                auto info = player.getPlayerInfo();

                cr_assert_eq(info["id"], "1", "The ID in getPlayerInfo is incorrect.");
                cr_assert_eq(info["health"], "100", "The health in getPlayerInfo is incorrect.");
                cr_assert_eq(info["x"], "0", "Position X in getPlayerInfo is incorrect.");
                cr_assert_eq(info["y"], "0", "The Y position in getPlayerInfo is incorrect.");
            }

            Test(Player, GetPosInfo)
            {
                boost::asio::io_context io_context;
                auto socket = std::make_shared<boost::asio::ip::tcp::socket>(boost::asio::make_strand(io_context));
                Player player(1, socket);

                std::string posInfo = player.getPosInfo();
                cr_assert(posInfo.find("x:0") != std::string::npos, "The X position information is incorrect in getPosInfo.");
                cr_assert(posInfo.find("y:0") != std::string::npos, "Position information Y is invalid in getPosInfo.");
            }

            /* ---- SHOOT CLASS ---- */

            Test(Shoot, ConstructorDefaultValues)
            {
                Shoot shoot(0, 0, RType::Game::Entity::ENTITY_TYPE::PLAYER, 100, 200, 5, 10, Direction::RIGHT);

                cr_assert_eq(shoot.getPosX(), 100, "Initial X position should be 100.");
                cr_assert_eq(shoot.getPosY(), 200, "Initial Y position should be 200.");
                cr_assert_eq(shoot.getDamage(), 10, "Initial damage should be 10.");
                cr_assert_eq(shoot.getIsActive(), true, "Shoot should be active after creation.");
            }

            Test(Shoot, UpdateMoveRight)
            {
                Shoot shoot(0, 0, RType::Game::Entity::ENTITY_TYPE::PLAYER, 100, 200, 5, 10, Direction::RIGHT);

                shoot.update();
                cr_assert_eq(shoot.getPosX(), 105, "Shoot should move 5 units to the right.");
                cr_assert_eq(shoot.getIsActive(), true, "Shoot should remain active when within screen bounds.");
            }

            Test(Shoot, UpdateMoveLeft)
            {
                Shoot shoot(0, 0, RType::Game::Entity::ENTITY_TYPE::PLAYER, 100, 200, 5, 10, Direction::LEFT);

                shoot.update();
                cr_assert_eq(shoot.getPosX(), 95, "Shoot should move 5 units to the left.");
                cr_assert_eq(shoot.getIsActive(), true, "Shoot should remain active when within screen bounds.");
            }

            Test(Shoot, UpdateOutOfBoundsRight)
            {
                Shoot shoot(0, 0, RType::Game::Entity::ENTITY_TYPE::PLAYER, 795, 200, 10, 10, Direction::RIGHT);

                shoot.update();
                cr_assert_eq(shoot.getIsActive(), false, "Shoot should deactivate when exceeding the right boundary.");
            }

            Test(Shoot, UpdateOutOfBoundsLeft)
            {
                Shoot shoot(0, 0, RType::Game::Entity::ENTITY_TYPE::PLAYER, 5, 200, 10, 10, Direction::LEFT);

                shoot.update();
                cr_assert_eq(shoot.getIsActive(), false, "Shoot should deactivate when exceeding the left boundary.");
            }

            Test(Shoot, SetterSetIsActive)
            {
                Shoot shoot(0, 0, RType::Game::Entity::ENTITY_TYPE::PLAYER, 100, 200, 5, 10, Direction::RIGHT);

                shoot.setIsActive(false);
                cr_assert_eq(shoot.getIsActive(), false, "Shoot should be inactive after setIsActive(false).");

                shoot.setIsActive(true);
                cr_assert_eq(shoot.getIsActive(), true, "Shoot should be active after setIsActive(true).");
            }

            Test(Shoot, GetterGetIsActive)
            {
                Shoot shoot(0, 0, RType::Game::Entity::ENTITY_TYPE::PLAYER, 100, 200, 5, 10, Direction::RIGHT);

                cr_assert_eq(shoot.getIsActive(), true, "Shoot should be active by default.");
            }

        } // namespace Entity
    } // namespace Game
} // namespace RType
