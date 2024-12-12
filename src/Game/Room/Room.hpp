/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Room class
** Caroline Boilly @Aeliondw
*/

#ifndef ROOM_HPP
    #define ROOM_HPP

#include <RType.hpp>


namespace Server
{
    class Room
    {
        enum class Mode {
            WAITING = 0,
            PLAYING = 1,
            END = 2
        };

        private:
            int _id;
            std::string _name;
            int count = 0;
            int mode = 0;

        public:
            Room(int id, std::shared_ptr<boost::asio::ip::tcp::socket> socket);
            ~Room() = default;

            /*  ---- SETTER ---- */
            void setColor(int color);

            /*  ---- GETTER ---- */
            std::unordered_map<std::string, int> getPlayerInfo() const;
            std::unordered_map<std::string, int> getPlayerSmallInfo() const;
            int getColor() const;
            bool getHaveJoined() const;
            std::shared_ptr<boost::asio::ip::tcp::socket> getSocket() const;

    };
} // namespace Server

#endif // ROOM_HPP
