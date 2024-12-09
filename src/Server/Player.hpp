//
// Created by dan13615 on 12/5/24.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <boost/asio.hpp>
#include <memory>

enum class Color {
    RED = 0,
    GREEN = 1,
    BLUE = 2,
    YELLOW = 3,
    PINK = 4,
    CYAN = 5,
    ORANGE = 6,
    PURPLE = 7,
    WHITE = 8
};

namespace Server
{

    class Player
    {
        private:
            int id_;
            typedef struct Position {
                    int x;
                    int y;
            } position_t;
            position_t position_;
            int color_;
            bool have_joined_ = false;
            std::shared_ptr<boost::asio::ip::tcp::socket> socket_;

        public:
            Player(int id, std::shared_ptr<boost::asio::ip::tcp::socket> socket);
            ~Player() = default;
            [[nodiscard]] int getId() const;
            [[nodiscard]] int getPosX() const;
            [[nodiscard]] int getPosY() const;
            [[nodiscard]] int getColor() const;
            [[nodiscard]] std::shared_ptr<boost::asio::ip::tcp::socket> getSocket() const;
            [[nodiscard]] position_t getPosition() const;
            void setPosX(int pos_x);
            void setPosY(int pos_y);
            void setColor(int color);
    };
} // namespace Server

#endif // PLAYER_HPP
