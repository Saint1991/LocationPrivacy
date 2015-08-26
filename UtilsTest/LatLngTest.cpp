
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
			Geography::LatLng latlng(1.0,2.0);//Geographyってつけましたが，これであってますか？
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
			Geography::LatLng latlng(1.0, 2.0);//Geographyってつけましたが，これであってますか？
			Assert::AreEqual(1.0, latlng.lat(), PRECISION);
			Assert::AreEqual(2.0, latlng.lng(), PRECISION);
		}

		/*
		TEST_METHOD(LatLng_dist)
		{
			Geography::LatLng latlng(1.0, 2.0);//Geographyってつけましたが，これであってますか？
			Assert::AreEqual(1.0, latlng.lat(), PRECISION);
			Assert::AreEqual(2.0, latlng.lng(), PRECISION);
		}
		TEST_METHOD(LatLng_angle)
		{
			Geography::LatLng latlng(1.0, 2.0);//Geographyってつけましたが，これであってますか？
			Assert::AreEqual(1.0, latlng.lat(), PRECISION);
			Assert::AreEqual(2.0, latlng.lng(), PRECISION);
		}
		*/



		
	};
}
