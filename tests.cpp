#include <iostream>
#include <doctest.h>
#include "flappy.h"


TEST_CASE("Upscore") {
    REQUIRE(Upscore(1) == 2);
    REQUIRE(Upscore(2) == 3);
    REQUIRE(Upscore(3) == 4);

}

