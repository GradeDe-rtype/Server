/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef RFC_ARG_PARSER_HPP_
#define RFC_ARG_PARSER_HPP_

/*  ---- INCLUDES ---- */
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>
#define MAX_ARGS_SIZE 256
#define MAX_COMMAND_SIZE 16

/*  ---- CLASS ---- */
class rfcArgParser
{

    public:
        struct DataPacket {
                char command[MAX_COMMAND_SIZE];
                char args[MAX_ARGS_SIZE];
        };
        /**
         * @brief PapayaError class
         *
         * This class is used to handle errors in the Papaya class
         */
        class Error : public std::exception
        {
            public:
                /**
                 * @brief Construct a new PapayaError object
                 *
                 * @param what The error message
                 * @param where The error location (default: "")
                 */
                Error(std::string const &what, std::string const &where = "");

                /**
                 * @brief Get the error message
                 *
                 * @return `const char *` The error message
                 */
                const char *what() const noexcept override;

                /**
                 * @brief Get the error location
                 *
                 * @return `const char *` The error location
                 */
                const char *where() const noexcept;

                /**
                 * @brief Read the error message
                 */
                void read() const noexcept;

            private:
                /**
                 * @brief The error message
                 */
                std::string _what;

                /**
                 * @brief The error location
                 */
                std::string _where;
        };

        /**
         * @brief Parse an array
         *
         * @param str The string to parse
         *
         * @return `std::vector<std::string>` The parsed array
         */
        static std::vector<std::string> ParseArray(std::string str);

        /**
         * @brief Parse an object
         *
         * @param str The string to parse
         *
         * @return `std::unordered_map<std::string, std::string>` The parsed object
         */
        static std::unordered_map<std::string, std::string> ParseObject(std::string str);

        /**
         * @brief Create a object
         *
         * @param obj The map containing all the object informations
         *
         * @return std::string Representing the created object
         */
        static std::string CreateObject(std::unordered_map<std::string, std::string> obj);

        /**
         * @brief Serialize a packet
         *
         * @param packet The DataPacket to serialize
         *
         * @return std::string Representing the packet deserialized
         */
        static std::string DeserializePacket(const DataPacket &packet);

        /**
         * @brief Deserialize a packet
         *
         * @param data The data to deserialize
         *
         * @return DataPacket Representing the data serialized
         */
        static DataPacket DeserializePacket(const std::string &data, const std::size_t length);

        /**
         * @brief Deserialize a packet
         *
         * @param data The data to deserialize
         *
         * @return DataPacket Representing the data serialized
         */
        static DataPacket SerializePacket(const std::string &command, const std::string &args);

    private:
        /**
         * @brief Split a string
         *
         * @param str The string to split
         * @param delimiter The delimiter
         *
         * @return `std::vector<std::string>` The splitted string
         */
        static std::vector<std::string> _split(std::string str, char delimiter);

        /**
         * @brief Keep a part of a string
         *
         * @param str The string to keep
         * @param start The start character
         * @param end The end character
         *
         * @return `std::string` The kept string
         */
        static std::string _keep(std::string str, char start, char end);
};

#endif /* RFC_ARG_PARSER_H_ */
