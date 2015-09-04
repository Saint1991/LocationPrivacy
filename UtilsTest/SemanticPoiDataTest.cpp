
#include "stdafx.h"
#include "CppUnitTest.h"
#include "SemanticPoiData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(SemanticPoiDataTest)
	{
	public:

		TEST_METHOD(SemanticPoiData_Constructor)
		{
			Geography::PoiData poi_data("osaka", "001", "city", Geography::LatLng(10.0,20.0));

			Assert::AreEqual(std::string("osaka"), poi_data.get_venue_name());
			Assert::AreEqual(std::string("001"), poi_data.get_category_id());
			Assert::AreEqual(std::string("city"), poi_data.get_category_name());
			Assert::AreEqual(10.0, poi_data.get_position().lat());
			Assert::AreEqual(20.0, poi_data.get_position().lng());

			
			Geography::PoiData poi_data2("osaka", "001", "city", 10.0, 20.0);

			Assert::AreEqual(std::string("osaka"), poi_data2.get_venue_name());
			Assert::AreEqual(std::string("001"), poi_data2.get_category_id());
			Assert::AreEqual(std::string("city"), poi_data2.get_category_name());
			Assert::AreEqual(10.0, poi_data2.get_position().lat());
			Assert::AreEqual(20.0, poi_data2.get_position().lng());

		}
	};
}
