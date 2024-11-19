cmake_minimum_required(VERSION 3.10)
project(rtype-server)

set(CMAKE_CXX_STANDARD 11)

# Boost ASIO is part of Boost Libraries
find_package(Boost REQUIRED)

# Include Boost headers
include_directories(${Boost_INCLUDE_DIRS})

# Add source files
add_executable(rtype-server main.cpp
        includes/server.hpp)

# Link Boost system and thread libraries
target_link_libraries(rtype-server ${Boost_LIBRARIES})
