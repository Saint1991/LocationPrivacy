#include "stdafx.h"
#include "CppUnitTest.h"
#include "Vector2d.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace UtilsTest
{
	TEST_CLASS(Vector2dTest)
	{
	public:
		
		const double precision = 10.e-10;

		TEST_METHOD(constructor)
		{
			double x = 3.2;
			double y = -1.2;
			Graph::Vector2d vec(x, y);
			Assert::AreEqual(x, vec.x());
			Assert::AreEqual(y, vec.y());
		}

		TEST_METHOD(length)
		{
			double x = -3.0;
			double y = 4.0;
			double expected = 5.0;

			Graph::Vector2d vec(x, y);
			double actual = vec.length();
			Assert::AreEqual(expected, actual, precision);
		}

		TEST_METHOD(rotate1)
		{
			double x = 1.0;
			double y = 0.0;
			Graph::Vector2d vec(x, y);

			double radian = M_PI / 2.0;
			vec.rotate(radian);

			Assert::AreEqual(0.0, vec.x(), precision);
			Assert::AreEqual(1.0, vec.y(), precision);
		}

		TEST_METHOD(rotate2)
		{
			double x = 1.0;
			double y = 0.0;
			Graph::Vector2d vec(x, y);

			double radian = -M_PI / 2.0;
			vec.rotate(radian);

			Assert::AreEqual(0.0, vec.x(), precision);
			Assert::AreEqual(-1.0, vec.y(), precision);
		}

		TEST_METHOD(operator_eq)
		{
			double x = 3.2;
			double y = 0.8;

			Graph::Vector2d vec1(x, y);
			Graph::Vector2d vec2(0.0, 0.0);

			vec2 = vec1;
			Assert::AreEqual(x, vec2.x());
			Assert::AreEqual(y, vec2.y());
		}

		TEST_METHOD(operator_pl)
		{
			double x1 = 3.2;
			double y1 = 0.8;

			double x2 = 1.8;
			double y2 = -1.8;
		
			Graph::Vector2d vec1(x1, y1);
			Graph::Vector2d vec2(x2, y2);

			Graph::Vector2d actual = vec1 + vec2;
			Assert::AreEqual(5.0, actual.x(), precision);
			Assert::AreEqual(-1.0, actual.y(), precision);
		}

		TEST_METHOD(operator_mi)
		{

			double x1 = 3.2;
			double y1 = 0.8;

			double x2 = 1.8;
			double y2 = -1.8;

			Graph::Vector2d vec1(x1, y1);
			Graph::Vector2d vec2(x2, y2);

			Graph::Vector2d actual = vec1 - vec2;
			Assert::AreEqual(1.4, actual.x(), precision);
			Assert::AreEqual(2.6, actual.y(), precision);
		}

		TEST_METHOD(operator_bl_eq1)
		{
			double x1 = 3.2;
			double y1 = 0.8;

			double x2 = 1.8;
			double y2 = -1.8;

			Graph::Vector2d vec1(x1, y1);
			Graph::Vector2d vec2(x2, y2);

			Assert::IsFalse(vec1 == vec2);
		}

		TEST_METHOD(operator_bl_eq2)
		{
			double x1 = 3.2;
			double y1 = 0.8;

			double x2 = 3.2;
			double y2 = 0.8;

			Graph::Vector2d vec1(x1, y1);
			Graph::Vector2d vec2(x2, y2);

			Assert::IsTrue(vec1 == vec2);
		}

		TEST_METHOD(operator_bl_neq1)
		{
			double x1 = 2.2;
			double y1 = 0.8;

			double x2 = 3.2;
			double y2 = 0.8;

			Graph::Vector2d vec1(x1, y1);
			Graph::Vector2d vec2(x2, y2);

			Assert::IsTrue(vec1 != vec2);
		}

		TEST_METHOD(operator_bl_neq2)
		{
			double x1 = 3.2;
			double y1 = 0.8;

			double x2 = 3.2;
			double y2 = 0.8;

			Graph::Vector2d vec1(x1, y1);
			Graph::Vector2d vec2(x2, y2);

			Assert::IsFalse(vec1 != vec2);
		}

		TEST_METHOD(inner_product)
		{
			double x1 = 2.2;
			double y1 = 0.8;

			double x2 = 3.2;
			double y2 = -1.8;

			double expected = 5.6;

			Graph::Vector2d vec1(x1, y1);
			Graph::Vector2d vec2(x2, y2);

			double actual = Graph::inner_product(vec1, vec2);
			Assert::AreEqual(actual, expected, precision);
		}

	};
}