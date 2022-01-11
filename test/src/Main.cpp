#include <gtest/gtest.h>

namespace SimpleJson_Test
{
	size_t g_numOfTestFile = 0;
}

int main(int argc, char** argv)
{
	constexpr size_t EXPECTED_NUM_OF_TEST_FILE = 11;

	std::cout << "===== SimpleJson test program =====" << std::endl;
	std::cout << std::endl;

	std::cout << std::endl;
	std::cout << "===== SimpleJson test start   =====" << std::endl;

	testing::InitGoogleTest(&argc, argv);
	int testRet = RUN_ALL_TESTS();

	if (SimpleJson_Test::g_numOfTestFile != EXPECTED_NUM_OF_TEST_FILE)
	{
		std::cout << "********************************************************************************" << std::endl;
		std::cout << "***** WARNING: Expecting " << EXPECTED_NUM_OF_TEST_FILE;
		std::cout << " testing source files, but only ";
		std::cout << SimpleJson_Test::g_numOfTestFile << " were ran. *****" << std::endl;
		std::cout << "********************************************************************************" << std::endl;

		return -1;
	}

	return testRet;
}
