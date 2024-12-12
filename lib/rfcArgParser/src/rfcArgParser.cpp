/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include "rfcArgParser.hpp"

std::vector<std::string> rfcArgParser::ParseArray(std::string str)
{
    std::vector<std::string> result;
    std::string tmp = _keep(str, '[', ']');
    if (tmp.size() == 0)
        throw rfcArgParser::Error("Invalid array format, missing \"[]\" or empty string", "rfcArgParser::ParseArray");

    tmp = tmp.substr(1, tmp.size() - 2);
    result = _split(tmp, ';');
    return result;
}

std::unordered_map<std::string, std::string> rfcArgParser::ParseObject(std::string str)
{
    std::unordered_map<std::string, std::string> result;
    std::string tmp = _keep(str, '{', '}');
    if (tmp.size() == 0)
        throw rfcArgParser::Error("Invalid object format, missing \"{}\" or empty string", "rfcArgParser::ParseObject");
    std::vector<std::string> tmpArray = _split(tmp, ',');

    for (std::string s : tmpArray) {
        std::vector<std::string> tmpPair = _split(s, ':');
        result[tmpPair[0]] = tmpPair[1];
    }
    return result;
}

std::string rfcArgParser::CreateObject(std::unordered_map<std::string, std::string> obj)
{
    if (obj.size() == 0)
        throw rfcArgParser::Error("Invalid object format, empty object", "rfcArgParser::CreateObject");
    std::string result = "{";
    for (auto it = obj.begin(); it != obj.end(); it++) {
        result += it->first + ":" + it->second + ",";
    }
    result.pop_back();
    result += "}";
    return result;
}

std::string rfcArgParser::DeserializePacket(const rfcArgParser::DataPacket &packet)
{
    std::string data = std::string(packet.command) + " " + packet.args;
    return data;
}

rfcArgParser::DataPacket rfcArgParser::DeserializePacket(const std::string &data, const std::size_t length)
{
    rfcArgParser::DataPacket packet{};
    memcpy(&packet, data.c_str(), length);
    return packet;
}

rfcArgParser::DataPacket rfcArgParser::SerializePacket(const std::string &command, const std::string &args)
{
    rfcArgParser::DataPacket packet{};
    strncpy(packet.command, command.c_str(), sizeof(packet.command) - 1);
    packet.command[sizeof(packet.command) - 1] = '\0';
    strncpy(packet.args, args.c_str(), sizeof(packet.args) - 1);
    packet.args[sizeof(packet.args) - 1] = '\0';
    return packet;
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
            continue;
        } else if (c == end) {
            keep = false;
        }
        if (keep) {
            result += c;
        }
    }
    return result;
}
