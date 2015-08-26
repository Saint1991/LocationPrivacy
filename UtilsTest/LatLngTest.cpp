
#include "stdafx.h"
#include "CppUnitTest.h"
#include "LatLng.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(LatLngTest)
	{
	public:
		const double PRECISION = 1.0E-10;

		TEST_METHOD(LatLng_Constructor1)
		{
			Geography::LatLng latlng(1.0,2.0);
			Assert::AreEqual(1.0, latlng.lat(), PRECISION);
			Assert::AreEqual(2.0, latlng.lng(), PRECISION);
		}

		TEST_METHOD(LatLng_Constructor2)
		{
			try
			{
				Geography::LatLng latlng(-100.0, 200.0);
			}
			catch (const std::exception&)
			{
				Assert::Fail();
			}
		}

		TEST_METHOD(LatLng_lat_and_lng)
		{
			Geography::LatLng latlng(1.0, 2.0);
			Assert::AreEqual(1.0, latlng.lat(), PRECISION);
			Assert::AreEqual(2.0, latlng.lng(), PRECISION);
		}

		
		TEST_METHOD(LatLng_dist)
		{
			Geography::LatLng latlng1(1.0, 2.0); 
			Geography::LatLng latlng2(10.0, 20.0);
			double latlng_d = dist(latlng1, latlng2);
			Assert::AreEqual(1.0, latlng_d, PRECISION);
		}

		TEST_METHOD(LatLng_angle)
		{
			Geography::LatLng latlng1(1.0, 2.0);
			Geography::LatLng latlng2(10.0, 20.0);
			double latlng_a = angle(latlng1, latlng2);//friendä÷êîÇ¡ÇƒâΩÅH
			Assert::AreEqual(1.0, latlng_a, PRECISION);
		}
		



		
	};
}
