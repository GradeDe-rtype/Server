cmake_minimum_required(VERSION 3.16)
project(RType VERSION 1.0)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define the list of libraries
set(RTYPE_LIBRARIES
    papaya
    rfcArgParser
)

# Include subdirectories for libraries
foreach(LIB ${RTYPE_LIBRARIES})
    add_subdirectory(lib/${LIB})
endforeach()

#  ---- FIND BOOST ---- #
find_package(Boost REQUIRED)

#  ---- FIND CRITERION ---- #
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(CRITERION REQUIRED criterion)

# Add tests
# add_subdirectory(tests)

# ---- SOURCES ---- #
file(GLOB_RECURSE SOURCES
    "src/**/**/**/**/*.cpp"
    "src/**/**/**/*.cpp"
    "src/**/**/*.cpp"
    "src/**/*.cpp"
    "src/*.cpp"
)

# ---- EXECUTABLE ---- #
add_executable(r-type_server ${SOURCES})

# ---- INCLUDES ---- #
target_include_directories(r-type_server PRIVATE
    "include/"
    "src/Game/"
    "src/Game/Colors/"
    "src/Game/Entity/"
    "src/Game/Entity/Base/"
    "src/Game/Entity/Monster/"
    "src/Game/Entity/Player/"
    "src/Game/Entity/Shoot/"
    "src/Game/Room/"
    "src/Game/Timer/"
    "src/Parsing/"
    "src/Server/"
    "src/Network/"
    "src/Utils/Utils/"
    "src/Utils/Path/"
)

include_directories(${Boost_INCLUDE_DIRS})

foreach(LIB ${RTYPE_LIBRARIES})
    target_include_directories(r-type_server PRIVATE "lib/${LIB}/include/")
endforeach()

# ---- LINK LIBRARIES ---- #
target_link_libraries(r-type_server PRIVATE
    ${RTYPE_LIBRARIES}
    ${Boost_LIBRARIES}
)

# ---- COMPILE OPTIONS ---- #
add_custom_command(TARGET r-type_server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:r-type_server> ${CMAKE_SOURCE_DIR}
)

# ---- CLEAN TARGET ---- #
add_custom_target(fclean
    COMMAND ${CMAKE_COMMAND} -E remove r-type_server
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/r-type_server
)
