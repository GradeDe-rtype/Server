/*
** EPITECH PROJECT, 2024
** tmp
** File description:
** Parsing
*/

#include "Parsing.hpp"

namespace RType {
    Parsing::ParsingError::ParsingError(const std::string &message)
        : _message(message)
    {
    }

    const char *Parsing::ParsingError::what() const noexcept
    {
        return _message.c_str();
    }
}