#include "stdafx.h"
#include "CppUnitTest.h"
#include "IDbSettingsLoader.h"
#include "DbSettingsFileLoader.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(DbSettingsLoaderTest)
	{
	public:

		TEST_METHOD(constructor1)
		{

			Db::DbSettingsFileLoader loader("C:/Users/Mizuno/Documents/Visual Studio 2015/Projects/LocationPrivacy/settings/dbsettings-sample.xml");
			Db::DbSettings settings = loader.load_db_settings();
			
			std::string host_expected = "localhost";
			std::string user_expected = "root";
			std::string password_expected = "root";
			Assert::AreEqual(host_expected, settings.hostname);
			Assert::AreEqual(3306U, settings.port);
			Assert::AreEqual(user_expected, settings.username);
			Assert::AreEqual(password_expected, settings.password);
		}


		///<summary>
		/// ポリモーフィズムの使い方はここが参考になる
		///</summary>
		TEST_METHOD(constructor2)
		{
			std::unique_ptr<Db::IDbSettingsLoader> loader = std::make_unique<Db::DbSettingsFileLoader>("C:/Users/Mizuno/Documents/Visual Studio 2015/Projects/LocationPrivacy/settings/dbsettings-sample.xml");
			Db::DbSettings settings = loader->load_db_settings();

			std::string host_expected = "localhost";
			std::string user_expected = "root";
			std::string password_expected = "root";
			Assert::AreEqual(host_expected, settings.hostname);
			Assert::AreEqual(3306U, settings.port);
			Assert::AreEqual(user_expected, settings.username);
			Assert::AreEqual(password_expected, settings.password);
		}
	};
}

