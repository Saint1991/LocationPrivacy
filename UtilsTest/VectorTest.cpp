#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{
	TEST_CLASS(VectorTest)
	{
	public:

		TEST_METHOD(vector_constructor)
		{
			const int count = 320000;
			std::unique_ptr<std::vector<std::shared_ptr<std::vector<double>>>> map = std::make_unique<std::vector<std::shared_ptr<std::vector<double>>>>(count, std::make_shared<std::vector<double>>(count, DBL_MAX));
			std::cout << "a";
		}
	};
}
