#include "stdafx.h"
#include "CppUnitTest.h"
#include "MysqlDb.h"
#include "DbSettingsFileLoader.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(MysqlDbTest)
	{
	public:

		TEST_METHOD(constructor1)
		{
			Db::MysqlDb db(std::make_unique<Db::DbSettingsFileLoader>(
				Db::DbSettingsFileLoader("C:/Users/Mizuno/Documents/Visual Studio 2015/Projects/LocationPrivacy/settings/mydbsettings.xml"))
			);
		}
	};
}
