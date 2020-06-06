#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "../src/bases.h"
int def(){
	return 2;
};
TEST_CASE("Weak and shared conversions","[classic]")
{
    REQUIRE(def() == 2);
    // REQUIRE(subtract(1, 1) == 0);
}