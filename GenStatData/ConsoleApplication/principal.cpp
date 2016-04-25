#define CATCH_CONFIG_RUNNER
#include "catch/catch.hpp"

int main(int argc, const char *argv[])
{
	// global setup...

	int result = Catch::Session().run(argc, argv);

	// global clean-up...

	return result;
}