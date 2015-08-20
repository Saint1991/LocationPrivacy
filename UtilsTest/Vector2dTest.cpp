
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Vector2d.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Graph;

namespace UtilsTest
{
	TEST_CLASS(Vector2dTest)
	{
	public:
		const double PRECISION = 1.0E-10;
		
		TEST_METHOD(Vector2dTest_Constructor1)
		{
			Vector2d v1(1.0, 2.0);

			Assert::AreEqual(1.0, v1.x());
			Assert::AreEqual(2.0, v1.y());
		}

		TEST_METHOD(Vector2dTest_Constructor2)
		{
			Coordinate c1(1.0, 2.0);
			Vector2d v1(c1);

			Assert::AreEqual(1.0, v1.x());
			Assert::AreEqual(2.0, v1.y());
		}
		
		TEST_METHOD(Vector2dTest_length)
		{
			Vector2d v1(1.0, 2.0);
			
			Assert::AreEqual(std::sqrt(5), v1.length());
		}
		
		TEST_METHOD(Vector2dTest_rotate)
		{
			Vector2d v1(1.0, 1.0);
			v1.rotate(M_PI_4);

			Assert::AreEqual(0.0, v1.x(), PRECISION);
			Assert::AreEqual(std::sqrt(2), v1.y(), PRECISION);
		}
		
		TEST_METHOD(Vector2dTest_operators1)
		{
			Vector2d v1(1.0, 2.0);
			Vector2d v2(2.0, 3.0);
			Vector2d v3 = v1 + v2;
			Vector2d v4 = v1 - v2;
			Vector2d v5(1.0, 2.0);

			Assert::AreEqual(3.0, v3.x(), PRECISION);
			Assert::AreEqual(5.0, v3.y(), PRECISION);
			Assert::AreEqual(-1.0, v4.x(), PRECISION);
			Assert::AreEqual(-1.0, v4.y(), PRECISION);
		}
		
		TEST_METHOD(Vector2dTest_operators2)
		{
			Vector2d v1(1.0, 2.0);
			Vector2d v2(2.0, 3.0);
			Vector2d v3(2.0, 3.0);

			Assert::IsFalse(v1 == v2);
			Assert::IsTrue(v2 == v3);
			Assert::IsTrue(v1 != v2);
			Assert::IsFalse(v2 != v3);
		}

		TEST_METHOD(Vector2dTest_inner_product)
		{
			Vector2d v1(1.0, 2.0);
			Vector2d v2(2.0, 3.0);
			double inner_p = inner_product(v1, v2);
			
			Assert::AreEqual(8.0, inner_p, PRECISION);
		}

		TEST_METHOD(Vector2dTest_cross_product)
		{
			Vector2d v1(1.0, 2.0);
			Vector2d v2(2.0, 3.0);
			double cross_p = cross_product(v1, v2);

			Assert::AreEqual(-1.0, cross_p, PRECISION);
		}

	};
}
