/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** Player class
** Daniil Stepanov @dan13615, Caroline Boilly @Aeliondw
*/

#ifndef RTYPE_GAME_COLORS_HPP_
#define RTYPE_GAME_COLORS_HPP_

#include <boost/asio.hpp>
#include <string>
#include <unordered_map>

namespace RType
{
    namespace Game
    {
        class Colors {
                public:
                    static Colors &get();

                    std::string getColor(int color) const;

                private:
                    Colors();

                    std::unordered_map<int, std::string> _colors;
            };
    } // namespace Game
} // namespace RType

#endif // RTYPE_GAME_COLORS_HPP_
