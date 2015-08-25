
#include "stdafx.h"
#include "CppUnitTest.h"
#include "DbSettings.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilisTest
{
	TEST_CLASS(DbSettingsTest)
	{
	public:

		TEST_METHOD(Dbsettings_Constructor)
		{
			std::string hostname = "nishio";
			std::string username = "abc";
			std::string pass = "240";

			Db::DbSettings db1(hostname, 1, username, pass);
			Assert::AreEqual(hostname, db1.hostname);
			Assert::AreEqual(1U, db1.port);
			Assert::AreEqual(username, db1.username);
			Assert::AreEqual(pass, db1.password);
		}
	};
}
