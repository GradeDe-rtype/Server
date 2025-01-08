# Tests CMakeLists.txt

file(GLOB TEST_SOURCES "*.cpp")

set(SRC_SOURCES
    ${CMAKE_SOURCE_DIR}/src/Game/Colors/Colors.cpp
)


foreach(FILE ${TEST_SOURCES} ${SRC_SOURCES})
    if (NOT EXISTS "${FILE}")
        message(FATAL_ERROR "Source file not found : ${FILE}")
    endif()
endforeach()

add_executable(r-type_tests ${TEST_SOURCES} ${SRC_SOURCES})

target_include_directories(r-type_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CRITERION REQUIRED criterion)

target_include_directories(r-type_tests PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(r-type_tests PRIVATE ${CRITERION_LIBRARIES})

target_link_libraries(r-type_tests PRIVATE
    ${Boost_LIBRARIES}
)

target_compile_options(r-type_tests PRIVATE -g -O0)

enable_testing()

add_test(
    NAME RTypeTests
    COMMAND $<TARGET_FILE:r-type_tests>
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Manual testing
add_custom_target(run_tests
    COMMAND $<TARGET_FILE:r-type_tests>
    DEPENDS r-type_tests
    COMMENT "Automatic Criterion test execution"
    VERBATIM
)
