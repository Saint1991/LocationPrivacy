#include "stdafx.h"
#include "CppUnitTest.h"
#include "AngleUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(AngleUtilityTest)
	{
	public:
		const double precision = 1.0e-10;

		TEST_METHOD(AngleUtility_convert_to_radian1)
		{
			double degree = 180;
			double expected = M_PI;
			
			double actual = Math::AngleUtility::convert_to_radian(degree);
			Assert::AreEqual(expected, actual, precision);
		}

		TEST_METHOD(AngleUtility_convert_to_radian2)
		{
			double degree = 135;
			double expected = 3.0 * M_PI / 4.0;

			double actual = Math::AngleUtility::convert_to_radian(degree);
			Assert::AreEqual(expected, actual, precision);
		}

		TEST_METHOD(AngleUtility_convert_to_degree1)
		{
			double radian = M_PI / 6.0;
			double expected = 30;

			double actual = Math::AngleUtility::convert_to_degree(radian);
			Assert::AreEqual(expected, actual, precision);
		}

		TEST_METHOD(AngleUtility_convert_to_degree2)
		{
			double radian = 3.0 * M_PI / 4.0;
			double expected = 135.0;

			double actual = Math::AngleUtility::convert_to_degree(radian);
			Assert::AreEqual(expected, actual, precision);
		}
	};
}