/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#include <Utils.hpp>

namespace RType
{
    namespace Utils
    {
        std::vector<std::string> split(const std::string &str, const std::string &delim)
        {
            if (str.empty())
                return {};
            if (str.find(delim) == std::string::npos)
                return {str};

            std::vector<std::string> tokens;
            std::size_t start = 0;

            while (true)
            {
                std::size_t end = str.find(delim, start);
                if (end == std::string::npos)
                {
                    tokens.push_back(str.substr(start));
                    break;
                }
                tokens.push_back(str.substr(start, end - start));
                start = end + delim.size();
                while (delim.find(str[start]) != std::string::npos && start < str.size())
                    start++;
            }
            return tokens;
        }

        bool DirectoryExists(const char *path)
        {
            struct stat info;

            if (stat(path, &info) != 0)
                return false;
            else if (info.st_mode & S_IFDIR)
                return true;
            return false;
        }

        bool isNumber(const std::string &str)
        {
            for (const auto &c : str)
                if (!std::isdigit(c) && c != '-')
                    return false;
            return true;
        }

        std::string trim(const std::string &str)
        {
            auto is_trim_char = [](unsigned char c)
            { return std::isspace(c) || c == '\n' || c == '\r' || c == '\t' || c == '\0'; };
            auto start = std::find_if_not(str.begin(), str.end(), is_trim_char);
            auto end = std::find_if_not(str.rbegin(), str.rend(), is_trim_char).base();
            return (start < end) ? std::string(start, end) : "";
        }

        std::string normalize(const std::string &str)
        {
            std::string result = trim(str);
            std::transform(result.begin(), result.end(), result.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            return result;
        }

        Server::DataPacket createDataPacket(const std::string &command, const std::string &args)
        {
            Server::DataPacket packet{};
            strncpy(packet.command, command.c_str(), sizeof(packet.command) - 1);
            packet.command[sizeof(packet.command) - 1] = '\0';
            strncpy(packet.args, args.c_str(), sizeof(packet.args) - 1);
            packet.args[sizeof(packet.args) - 1] = '\0';
            return packet;
        }
    } // namespace Utils
} // namespace RType
