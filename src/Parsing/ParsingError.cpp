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
    Parsing::ParsingError::ParsingError(const std::string &message) : _message(message) {}

    const char *Parsing::ParsingError::what() const noexcept { return _message.c_str(); }
} // namespace RType
