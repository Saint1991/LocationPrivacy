#include "stdafx.h"
#include "CppUnitTest.h"
#include "DbSettingsFileLoader.h"
#include "DbSettings.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(DbSettingsFileLoaderTest)
	{
	public:

		TEST_METHOD(DbSettingsFileLoader_load_db_settings)
		{
			std::string path = "../settings/dbsettings-sample.xml";
			const Db::DbSettingsFileLoader loader(path);
			Db::DbSettings settings = loader.load_db_settings();

			Assert::AreEqual(std::string("localhost"), settings.hostname);
			Assert::AreEqual(3306U, settings.port);
			Assert::AreEqual(std::string("root"), settings.username);
			Assert::AreEqual(std::string("root"), settings.password);
		}
	};
}

