/*
** EPITECH PROJECT, 2024
** rfc-arg-parser-library
** File description:
** rfcArgParser_private
*/

#include "rfcArgParser.hpp"

std::vector<std::string> rfcArgParser::ParseArray(std::string str)
{
    std::vector<std::string> result;
    std::string tmp = _keep(str, '[', ']');
    if (tmp.size() == 0)
        throw rfcArgParser::Error("Invalid array format, missing brackets", "rfcArgParser::ParseArray");

    tmp = tmp.substr(1, tmp.size() - 2);
    result = _split(tmp, ';');
    return result;
}

std::unordered_map<std::string, std::string> rfcArgParser::ParseObject(std::string str)
{
    std::unordered_map<std::string, std::string> result;
    std::string tmp = _keep(str, '{', '}');
    tmp = tmp.substr(1, tmp.size() - 2);
    if (tmp.size() == 0)
        throw rfcArgParser::Error("Invalid object format, missing brackets", "rfcArgParser::ParseObject");
    std::vector<std::string> tmpArray = _split(tmp, ',');

    for (std::string s : tmpArray) {
        std::vector<std::string> tmpPair = _split(s, ':');
        result[tmpPair[0]] = tmpPair[1];
    }
    return result;
}

std::vector<std::string> rfcArgParser::_split(std::string str, char delimiter)
{
    std::vector<std::string> result;
    std::string tmp = "";

    for (char c : str) {
        if (c == delimiter) {
            result.push_back(tmp);
            tmp = "";
        } else {
            tmp += c;
        }
    }
    result.push_back(tmp);
    return result;
}

std::string rfcArgParser::_keep(std::string str, char start, char end)
{
    std::string result = "";
    bool keep = false;

    for (char c : str) {
        if (c == start) {
            keep = true;
        } else if (c == end) {
            keep = false;
        }
        if (keep) {
            result += c;
        }
    }
    return result;
}