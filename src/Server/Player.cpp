//
// Created by dan13615 on 12/5/24.
//

#include "Player.hpp"

namespace Server {

    Player::Player(int id, std::shared_ptr<boost::asio::ip::tcp::socket> socket) : id_(id), socket_(socket) {
        pos_x_ = 0;
        pos_y_ = 0;
        color_ = 0;
    }

    int Player::getId() const {
        return id_;
    }

    int Player::getPosX() const {
        return pos_x_;
    }

    int Player::getPosY() const {
        return pos_y_;
    }

    int Player::getColor() const {
        return color_;
    }

    std::shared_ptr<boost::asio::ip::tcp::socket> Player::getSocket() const {
        return socket_;
    }

    void Player::setPosX(int pos_x) {
        pos_x_ = pos_x;
    }

    void Player::setPosY(int pos_y) {
        pos_y_ = pos_y;
    }

    void Player::setColor(int color) {
        color_ = color;
    }
}