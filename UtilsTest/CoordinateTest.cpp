
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Coordinate.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Graph;

namespace UtilisTest
{
	TEST_CLASS(CoordinateTest)
	{
	public:

		TEST_METHOD(Coordinate_Constructor)
		{
			Coordinate c1(1.0, 2.0);
			Assert::AreEqual(1.0, c1.x());
			Assert::AreEqual(2.0, c1.y());
		}

		TEST_METHOD(Coordinate_operators)
		{
			Coordinate c2(1.0, 2.0);
			Coordinate c3(2.0, 4.0);
			Coordinate c4(3.0, 6.0);
			Coordinate c5(-1.0, -2.0);

			//operatorの前後はスペース入れてね
			//Assert::IsTrue(c5 == c2 - c3);

		}
	};
}
