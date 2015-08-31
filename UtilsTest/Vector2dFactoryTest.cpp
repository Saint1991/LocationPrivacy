#include "stdafx.h"
#include "CppUnitTest.h"
#include "Vector2dFactory.h"
#include "Vector2d.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilisTest
{
	TEST_CLASS(Vector2dFactoryTest)
	{
	public:
		const double ACCURACY = 1.0E-10;

		TEST_METHOD(Vector2dFactory_create_by_cartesian)
		{
			Graph::Vector2d vf1 = Graph::Vector2dFactory::create_by_cartesian(1.0, 2.0);
			Assert::AreEqual(1.0, vf1.x(), ACCURACY);
			Assert::AreEqual(2.0, vf1.y(), ACCURACY);
		}

		TEST_METHOD(Vector2dFactory_create_by_polar)
		{
			Graph::Vector2d vf1 = Graph::Vector2dFactory::create_by_polar(2.0, M_PI_4);
			Assert::AreEqual(std::sqrt(2.0), vf1.x(), ACCURACY);
			Assert::AreEqual(std::sqrt(2.0), vf1.y(), ACCURACY);
		}
	};
}
