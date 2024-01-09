#include "../include/arithmetics.h"
#include "catch2/catch_all.hpp"

TEST_CASE("Getting operator priority 1")
{
    REQUIRE(getOperatorPriority('+') == 1);
    REQUIRE(getOperatorPriority('+') == getOperatorPriority('-'));

    REQUIRE(getOperatorPriority('*') == 2);
    REQUIRE(getOperatorPriority('*') == getOperatorPriority('/'));
    REQUIRE(getOperatorPriority('*') == getOperatorPriority('%'));

    REQUIRE(getOperatorPriority('d') == 0);

}

TEST_CASE("Getting operator priority 2")
{
    REQUIRE(getOperatorPriority("+") == 1);
    REQUIRE(getOperatorPriority("+") == getOperatorPriority("-"));

    REQUIRE(getOperatorPriority("*") == 2);
    REQUIRE(getOperatorPriority("*") == getOperatorPriority("/"));
    REQUIRE(getOperatorPriority("*") == getOperatorPriority("%"));

    REQUIRE(getOperatorPriority("d") == 0);

    REQUIRE(getOperatorPriority("+") == getOperatorPriority('+'));

}

TEST_CASE("Calculate two number operation")
{
    REQUIRE(computeExpression('z')(2, 3) == -1);

    REQUIRE(computeExpression('+')(12, 32) == 44);
    REQUIRE(computeExpression('-')(19, 9) == 10);
    
    REQUIRE(computeExpression('*')(20, 3) == 60);
    REQUIRE(computeExpression('/')(10, 3) == 3);
    REQUIRE(computeExpression('%')(12, 5) == 2);
}