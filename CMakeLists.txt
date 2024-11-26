cmake_minimum_required(VERSION 3.10)
project(server)

set(CMAKE_CXX_STANDARD 11)

# Boost ASIO is part of Boost Libraries
find_package(Boost REQUIRED)

# Include Boost headers
include_directories(${Boost_INCLUDE_DIRS})
include_directories(includes)

# Add source files
add_executable(server main.cpp
        src/server.cpp
        src/command.cpp
        src/utils.cpp)

# Link Boost system and thread libraries
target_link_libraries(server ${Boost_LIBRARIES})
