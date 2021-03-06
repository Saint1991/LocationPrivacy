
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
		const double ACCURACY = 1.0E-10;

		TEST_METHOD(Coordinate_Constructor)
		{
			Coordinate c1(1.0, 2.0);
			Assert::AreEqual(1.0, c1.x());
			Assert::AreEqual(2.0, c1.y());
			Assert::AreEqual(0U, c1.get_id());
		}

		TEST_METHOD(Coordinate_Constructor2)
		{
			Coordinate c1(1.0, 2.0, 3);
			Assert::AreEqual(3U, c1.get_id());
		}

		TEST_METHOD(Coordinate_operators)
		{
			Coordinate c1(1.0, 2.0);
			Coordinate c2(2.0, 4.0);
			Coordinate c3 = c1 + c2;
			Coordinate c4 = c1 - c2;
			
			Assert::AreEqual(3.0, c3.x(), ACCURACY);
			Assert::AreEqual(6.0, c3.y(), ACCURACY);
			Assert::AreEqual(-1.0, c4.x(), ACCURACY);
			Assert::AreEqual(-2.0, c4.y(), ACCURACY);
		}

		TEST_METHOD(Coordinate_dist)
		{
			Coordinate c1(1.0, 2.0);
			Coordinate c2(2.0, 4.0);
			double actual = dist(c1, c2);

			Assert::AreEqual(std::sqrt(5), actual);
		}

		TEST_METHOD(Coordinate_dist2)
		{
			Coordinate c1(1.0, 0.0);
			Coordinate c2(-1.0, 0.0);
			double actual = dist(c1, c2);

			Assert::AreEqual(2.0, actual);
		}

		
	};
}
