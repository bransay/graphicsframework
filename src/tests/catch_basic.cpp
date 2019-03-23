#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

SCENARIO("Testing passing functionality of catch2", "[catch]")
{
	WHEN("We should be passing a test")
	{
		THEN("The test passes")
		{
			REQUIRE(true);
		}
	}
}

SCENARIO("Testing failure functionality of catch2", "[catch][!shouldfail][.]")
{
	WHEN("We should be failing a test")
	{
		THEN("The test fails")
		{
			REQUIRE(false);
		}
	}
}
