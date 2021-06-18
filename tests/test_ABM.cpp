#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "cppyabm/bases.h"
int def(){
	return 2;
};
TEST_CASE("Just check","[classic]")
{
    REQUIRE(def() == 2);
    ;
}
