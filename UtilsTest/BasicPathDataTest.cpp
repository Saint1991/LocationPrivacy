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

		const double precision = 0.1;

		TEST_METHOD(BasicPathData_Constructors_and_get_distance)
		{
			BasicPathData data1(0.2);
			double get_d1 = data1.get_distance();

			Assert::AreEqual(0.2, get_d1, precision);


			Coordinate c1(1.0, 2.0, 1U);
			Coordinate c2(2.0, 3.0, 2U);
			BasicPathData data2(c1, c2);
			double get_d2 = data2.get_distance();

			Assert::AreEqual(std::sqrt(2.0), get_d2, precision);


			Geography::LatLng latlng1(1.0, 2.0);
			Geography::LatLng latlng2(10.0, 20.0);
			BasicPathData data3(latlng1, latlng2);
			double get_d3 = data3.get_distance();

			//Assert::AreEqual(std::sqrt(2.0), data3.get_distance(), precision);
			
		}

		TEST_METHOD(BasicPathData_update_distance)
		{
			BasicPathData data1(0.2);
			data1.update_distance(2.0);
			double get_d1 = data1.get_distance();


			Coordinate c1(1.0, 2.0, 1U);
			Coordinate c2(2.0, 3.0, 2U);
			BasicPathData data2(c1, c2);
			data2.update_distance(10.0);
			double get_d2 = data2.get_distance();


			Geography::LatLng latlng1(1.0, 2.0);
			Geography::LatLng latlng2(10.0, 20.0);
			BasicPathData data3(latlng1, latlng2);
			data3.update_distance(20.0);
			double get_d3 = data3.get_distance();


			Assert::AreEqual(2.0, get_d1, precision);
			Assert::AreEqual(10.0, get_d2, precision); 
			Assert::AreEqual(20.0, get_d3, precision);
		}
	};
}
