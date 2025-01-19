# Tests CMakeLists.txt

#  ---- SOURCES ---- #
file(GLOB TEST_SOURCES "*.cpp")

#  ---- SRC SOURCES ---- #
file(GLOB_RECURSE SRC_SOURCES
    "${CMAKE_SOURCE_DIR}/src/**/*.cpp"
)

foreach(FILE ${TEST_SOURCES} ${SRC_SOURCES})
    if (NOT EXISTS "${FILE}")
        message(FATAL_ERROR "Source file not found : ${FILE}")
    endif()
endforeach()

# ---- EXECUTABLE ---- #
add_executable(r-type_tests ${TEST_SOURCES} ${SRC_SOURCES})

# ---- INCLUDES ---- #
target_include_directories(r-type_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/src/Game/
    ${CMAKE_SOURCE_DIR}/src/Game/Colors/
    ${CMAKE_SOURCE_DIR}/src/Game/Entity/
    ${CMAKE_SOURCE_DIR}/src/Game/Entity/Base/
    ${CMAKE_SOURCE_DIR}/src/Game/Entity/Monster/
    ${CMAKE_SOURCE_DIR}/src/Game/Entity/Player/
    ${CMAKE_SOURCE_DIR}/src/Game/Entity/Shoot/
    ${CMAKE_SOURCE_DIR}/src/Game/Room/
    ${CMAKE_SOURCE_DIR}/src/Game/Timer/
    ${CMAKE_SOURCE_DIR}/src/Parsing/
    ${CMAKE_SOURCE_DIR}/src/Server/
    ${CMAKE_SOURCE_DIR}/src/Network/
    ${CMAKE_SOURCE_DIR}/src/Utils/Utils/
    ${CMAKE_SOURCE_DIR}/src/Utils/Path/
    ${CMAKE_SOURCE_DIR}/lib/papaya/include/
    ${CMAKE_SOURCE_DIR}/lib/rfcArgParser/include/
)

target_include_directories(r-type_tests PRIVATE 
    ${CRITERION_INCLUDE_DIRS}
)

# ---- LINK LIBRARIES ---- #
target_link_libraries(r-type_tests PRIVATE
    ${Boost_LIBRARIES}
    ${CRITERION_LIBRARIES}
    papaya
    rfcArgParser
)

# ---- COMPILE OPTIONS ---- #
target_compile_options(r-type_tests PRIVATE -g -O0)

# ---- TESTS ---- #
enable_testing()

# ---- ADD TEST ---- #
add_test(
    NAME RTypeTests
    COMMAND $<TARGET_FILE:r-type_tests>
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# ---- MANUAL TESTS EXECUTION ---- #
add_custom_target(run_tests
    COMMAND $<TARGET_FILE:r-type_tests>
    DEPENDS r-type_tests
    COMMENT "Manual Criterion test execution"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    VERBATIM
)
