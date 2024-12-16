/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen
    >> Daniil Stepanov @dan13615

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include "Colors.hpp"

namespace RType
{
    namespace Game
    {
        Colors &Colors::get()
        {
            static Colors instance;
            return instance;
        }

        Colors::Colors()
        {
            _colors[0] = "#FF0000";
            _colors[1] = "#00FF00";
            _colors[2] = "#0000FF";
            _colors[3] = "#FFFF00";
            _colors[4] = "#FF00FF";
            _colors[5] = "#00FFFF";
            _colors[6] = "#FFA500";
            _colors[7] = "#800080";
        }

        std::string Colors::getColor(int color) const
        {
            return _colors.at(color % _colors.size());
        }
    } // namespace Game
} // namespace RType
