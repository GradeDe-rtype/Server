//
// Created by dan13615 on 11/26/24.
//

#ifndef UTILS_HPP
    #define UTILS_HPP

#include <algorithm>
#include <cctype>
#include <string>

namespace Server {
    std::string trim(const std::string& str);
    std::string normalize(const std::string& str);
}
#endif //UTILS_HPP
