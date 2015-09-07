
#include "stdafx.h"
#include "CppUnitTest.h"
#include "BasicPoiData.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(BasicPoiDataTest)
	{
	public:

		TEST_METHOD(BasicPoiData_Constructors)
		{
			Geography::BasicPoiData poi_data1("osaka", Geography::LatLng(10.0, 20.0));

			Assert::AreEqual(std::string("osaka"), poi_data1.get_venue_name());
			Assert::AreEqual(10.0, poi_data1.get_position().lat());
			Assert::AreEqual(20.0, poi_data1.get_position().lng());


			Geography::BasicPoiData poi_data2("osaka", 10.0, 20.0);

			Assert::AreEqual(std::string("osaka"), poi_data2.get_venue_name());
			Assert::AreEqual(10.0, poi_data2.get_position().lat());
			Assert::AreEqual(20.0, poi_data2.get_position().lng());
		}
			
	};
}
