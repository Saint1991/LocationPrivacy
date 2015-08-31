#include "stdafx.h"
#include "CppUnitTest.h"
#include "BasicPathData.h"
#include "Coordinate.h"
#include "LatLng.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Graph;

namespace UtilsTest
{
	TEST_CLASS(BasicPathDataTest)
	{
	public:

		const double ACCURACY = 0.1;

		TEST_METHOD(BasicPathData_Constructor1)
		{
			BasicPathData data1(0.2);
			Assert::AreEqual(0.2, data1.get_distance(), ACCURACY);
		}

		TEST_METHOD(BasicPathData_Constructor2)
		{
			Coordinate c1(1.0, 2.0);
			Coordinate c2(2.0, 3.0);
			BasicPathData data2(c1, c2);
			Assert::AreEqual(std::sqrt(2.0), data2.get_distance(), ACCURACY);
		}

		TEST_METHOD(BasicPathData_Constructor3)
		{
			Geography::LatLng latlng1(1.0, 2.0);
			Geography::LatLng latlng2(10.0, 20.0);
			BasicPathData data3(latlng1, latlng2);
			double expected = 2227169.605;

			Assert::AreEqual(expected, data3.get_distance(), expected * ACCURACY);
		}

		

		TEST_METHOD(BasicPathData_update_distance1)
		{
			BasicPathData data(0.2);
			data.update_distance(2.0);
			double distance = data.get_distance();
			Assert::AreEqual(2.0, distance, ACCURACY);
		}

		TEST_METHOD(BasicPathData_update_distance2)
		{
			Coordinate c1(1.0, 2.0);
			Coordinate c2(2.0, 3.0);
			BasicPathData data(c1, c2);
			data.update_distance(10.0);
			double distance = data.get_distance();
			Assert::AreEqual(10.0, distance, ACCURACY);
		}

		TEST_METHOD(BasicPathData_update_distance3)
		{

			Geography::LatLng latlng1(1.0, 2.0);
			Geography::LatLng latlng2(10.0, 20.0);
			BasicPathData data(latlng1, latlng2);
			data.update_distance(20.0);
			double distance = data.get_distance();
			Assert::AreEqual(20.0, distance, ACCURACY);
		}
	};
}
