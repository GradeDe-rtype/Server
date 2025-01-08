#include <criterion/criterion.h>
#include <criterion/logging.h>

#include "../src/Game/Colors/Colors.hpp"

using namespace RType::Game;

// Colors class

Test(Colors, GetValidColors) {
    Colors &colors = Colors::get();

    cr_assert_eq(colors.getColor(0), "#FF0000", "The color at index 0 should be #FF0000");
    cr_assert_eq(colors.getColor(1), "#00FF00", "The color at index 1 should be #00FF00");
    cr_assert_eq(colors.getColor(2), "#0000FF", "The color at index 2 should be #0000FF");
    cr_assert_eq(colors.getColor(3), "#FFFF00", "The color at index 3 should be #FFFF00");
}

Test(Colors, GetColorOutOfBounds) {
    Colors &colors = Colors::get();

    cr_assert_eq(colors.getColor(8), "#FF0000", 
        "Index 8 should loop and return the color of index 0 (#FF0000)");
    cr_assert_eq(colors.getColor(9), "#00FF00", 
        "Index 9 should loop and return the color of index 1 (#00FF00)");
}

Test(Colors, SingletonInstance) {
    Colors &instance1 = Colors::get();
    Colors &instance2 = Colors::get();

    cr_assert_eq(&instance1, &instance2, "Both instances should point to the same Colors object");
}

Test(Colors, LargeIndexTest) {
    Colors &colors = Colors::get();
    
    cr_assert_eq(colors.getColor(15), "#800080", 
        "Index 15 should loop properly (15 % 8 = 7 -> #800080)");
    cr_assert_eq(colors.getColor(20), "#FF00FF", 
        "Index 20 should loop properly (20 % 8 = 4 -> #FF00FF)");
}
