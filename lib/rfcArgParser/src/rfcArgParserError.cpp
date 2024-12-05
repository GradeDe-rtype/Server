/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include "rfcArgParser.hpp"

rfcArgParser::Error::Error(std::string const &what, std::string const &where)
{
    _what = what;
    _where = where;
}

const char *rfcArgParser::Error::what() const noexcept
{
    return _what.c_str();
}

const char *rfcArgParser::Error::where() const noexcept
{
    return _where.c_str();
}

void rfcArgParser::Error::read() const noexcept
{
    std::cerr << "rfcArgParser Error";
    if (_where != "")
        std::cerr << " at " << _where;
    std::cerr << ": " << std::endl;
    std::cerr << ">> " << _what << std::endl;
}
