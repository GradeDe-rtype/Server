/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef RTYPE_UTILS_HPP_
#define RTYPE_UTILS_HPP_

/*  ---- INCLUDES ---- */
#include <DataPacket.hpp>
#include "RType.hpp"

/*  ---- CLASS ---- */
namespace RType
{
    namespace Utils
    {
        /**
         * @brief Split a string into a vector of strings
         *
         * @param str The string to split
         * @param delimiter The delimiter to split the string with
         *
         * @return A vector of strings
         */
        std::vector<std::string> split(const std::string &str, const std::string &delimiter);

        /**
         * @brief Check if a directory exists
         *
         * @param path The path to the directory
         *
         * @return true if the directory exists, false otherwise
         */
        bool DirectoryExists(const char *path);

        /**
         * @brief Check if a string is made of only numbers
         *
         * @param str The string to check
         *
         * @return `true` if the string is made of only numbers, `false` otherwise
         */
        bool isNumber(const std::string &str);

        /**
         * @brief Trim a string
         *
         * @param str The string to trim
         *
         * @return The trimmed string
         */
        std::string trim(const std::string &str);

        /**
         * @brief Normalize a string
         *
         * @param str The string to normalize
         *
         * @return The normalized string
         */
        std::string normalize(const std::string &str);

        /**
         * @brief Create a DataPacket
         *
         * @param command The command to put in the DataPacket
         * @param args The arguments to put in the DataPacket
         *
         * @return The created DataPacket
         */
        Server::DataPacket createDataPacket(const std::string &command, const std::string &args);
    } // namespace Utils
} // namespace RType

#endif /* !RTYPE_UTILS_HPP_ */
