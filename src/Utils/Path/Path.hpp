/*
    Authors:
    >> Nathan TIROLF - { nathan.tirolf@epitech.eu }

    („• ֊ •„)❤  <  Have a good day !
    --U-----U------------------------
*/

#ifndef RTYPE_PATH_HPP_
    #define RTYPE_PATH_HPP_

    /*  ---- INCLUDES ---- */
    #include <string>

    #include "Utils.hpp"


    /*  ---- CLASS ---- */
namespace RType {
    namespace Utils {
        class Path {
            public:
                static Path *get();
                std::string path() const;

            private:
                Path();
                ~Path() = default;

                std::string _path;
        };
    }
}

#endif /* !RTYPE_PATH_HPP_ */
