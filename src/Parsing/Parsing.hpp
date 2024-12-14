/*
    Authors:
    >> Caroline Boilly @Aeliondw
    >> Nathan Tirolf @dragusheen
    >> Daniil Stepanov @dan13615

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef RTYPE_PARSING_HPP_
#define RTYPE_PARSING_HPP_

/*  ---- INCLUDES ---- */
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include "Papaya.hpp"
#include "Path.hpp"
#include "Utils.hpp"

/*  ---- CLASS ---- */
namespace RType
{
    /**
     * @brief Parsing class
     */
    class Parsing
    {
        public:
            /**
             * @brief Parsing error class
             *
             * @note This class is used to throw parsing errors
             */
            class ParsingError : public std::exception
            {
                public:
                    ParsingError(const std::string &message);
                    const char *what() const noexcept override;

                private:
                    std::string _message;
            };

            /**
             * @brief Help class
             *
             * @note This class is used to throw help errors
             */
            class Help : public std::exception
            {
                public:
                    Help() = default;
                    const char *what() const noexcept override;
            };

            /**
             * @brief Constructor of Parsing class
             *
             * @param argc The number of arguments
             * @param argv The arguments
             */
            Parsing(int argc, char **argv);
            ~Parsing() = default;

            /**
             * @brief Get the port
             *
             * @return `int` The port
             *
             * @warning This function have to be used after the constructor. Else it will
             * return -1
             */
            int getPort();

        protected:

        private:
            /**
             * @brief Parse the port argument
             *
             * @param argc The number of arguments
             * @param argv The arguments
             * @param i The index of the argument
             */
            void _parseArgPort(int argc, char **argv, int i);

            void _parseNoArgs();

            int _port = -1;
    };
} // namespace RType

#endif /* !RTYPE_PARSING_HPP_ */
