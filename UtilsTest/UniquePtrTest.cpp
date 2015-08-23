#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(UniquePtrTest)
	{
	public:

		TEST_METHOD(move)
		{
			std::unique_ptr<double> d1 = std::make_unique<double>(1.2);
			std::unique_ptr<double> d2 = std::make_unique<double>(1.3);
			std::unique_ptr<double> d3 = std::move(d1);
			d2 = std::move(d1);
			if (d2 == nullptr) {
				Logger::WriteMessage("d2 is nullptr");
			}
		}
	};
}
