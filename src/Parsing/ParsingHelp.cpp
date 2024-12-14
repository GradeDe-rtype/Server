/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen
    >> Daniil Stepanov @dan13615

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include "Parsing.hpp"

namespace RType
{
    const char *Parsing::Help::what() const noexcept
    {
        std::ifstream file(Utils::Path::get()->path() + "data/help.txt");
        if (!file.is_open())
            return "Failed to open help file.";
        std::string content;
        std::string line;
        while (std::getline(file, line))
            content += line + "\n";
        file.close();
        std::cout << content << std::endl;
        return "";
    }
} // namespace RType
