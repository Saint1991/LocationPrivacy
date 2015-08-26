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

		TEST_METHOD(BasicPathData_Constructors)
		{
			BasicPathData data1(0.2);
			
			Assert::AreEqual(0.2, data1.get_distance(), precision);

			Coordinate c1(1.0,2.0,1U);
			Coordinate c2(2.0,3.0,2U);
			BasicPathData data2(c1, c2);

			Geography::LatLng latlng1(1.0,2.0);
			Geography::LatLng latlng2(10.0, 20.0);
			BasicPathData data3(latlng1, latlng2);

		}

	};
}
