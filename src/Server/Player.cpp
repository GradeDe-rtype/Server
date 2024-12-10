//
// Created by dan13615 on 12/5/24.
//

#include "Player.hpp"

namespace Server
{

    Player::Player(int id, std::shared_ptr<boost::asio::ip::tcp::socket> socket) : id_(id), socket_(socket)
    {
        position_ = {0, 0};
        color_ = 0;
    }

    int Player::getId() const { return id_; }

    int Player::getPosX() const { return position_.x; }

    int Player::getPosY() const { return position_.y; }

    Player::position_t Player::getPosition() const { return position_; }

    int Player::getColor() const { return color_; }

    std::shared_ptr<boost::asio::ip::tcp::socket> Player::getSocket() const { return socket_; }

    void Player::setPosX(int pos_x) { position_.x = pos_x; }

    void Player::setPosY(int pos_y) { position_.y = pos_y; }

    void Player::setColor(int color) { color_ = color; }
} // namespace Server
