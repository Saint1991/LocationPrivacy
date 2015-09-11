#include "stdafx.h"
#include "CppUnitTest.h"
#include "BasicRoad.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(BasicPathTest)
	{
	public:
		const double ACCURACY = 1.0E-10;
		
		TEST_METHOD(BasicRoad_Constructor)
		{
			Map::BasicRoad road(1L, 100.0);
			Assert::AreEqual(1L, road.get_to(), ACCURACY);
			Assert::AreEqual(100.0, road.distance(), ACCURACY);
		}

		TEST_METHOD(BasicRoad_update_distance)
		{
			Map::BasicRoad road(1L, 100.0);
			road.update_distance(50.0);
			Assert::AreEqual(50.0, road.distance(), ACCURACY);
		}
	};
}
