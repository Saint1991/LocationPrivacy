#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(PointerTest)
	{
	public:
		TEST_METHOD(move)
		{
			int* a = nullptr;
			int* b = std::move(a);
			Assert::IsTrue(b == nullptr);
		}
	};
}
