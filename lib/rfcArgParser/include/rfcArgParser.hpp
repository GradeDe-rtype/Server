/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef RFC_ARG_PARSER_HPP_
#define RFC_ARG_PARSER_HPP_

/*  ---- INCLUDES ---- */
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

/*  ---- CLASS ---- */
class rfcArgParser
{
    public:
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
