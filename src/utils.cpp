//
// Created by dan13615 on 11/26/24.
//

#include "../includes/utils.hpp"

namespace Server {

    std::string trim(const std::string& str) {
        auto is_trim_char = [](unsigned char c) {
            return std::isspace(c) || c == '\n' || c == '\r' || c == '\t' || c == '\0';
        };
        auto start = std::find_if_not(str.begin(), str.end(), is_trim_char);
        auto end = std::find_if_not(str.rbegin(), str.rend(), is_trim_char).base();
        return (start < end) ? std::string(start, end) : "";
    }

    std::string normalize(const std::string& str) {
        std::string result = trim(str);
        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        return result;
    }
};