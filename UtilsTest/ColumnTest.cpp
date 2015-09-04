#include "stdafx.h"
#include "CppUnitTest.h"
#include "Column.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(ColumnTest)
	{
	public:

		TEST_METHOD(Column_Constructor)
		{
			std::string column_name = "nishio";
			std::string type = "VARCHAR(32)";
			std::list<std::string> options = { "PRIMARY KEY", "AUTO INCREMENT" };
			std::string default_value = "teacher";
			Db::Column column(column_name, type, options, default_value);

			Assert::AreEqual(column_name, column.column_name);
			Assert::AreEqual(type, column.data_type);


			std::list<std::string>::const_iterator iter2 = column.options->begin();
			for (std::list<std::string>::const_iterator iter = options.begin();
			iter != options.end() && iter2 != column.options->end();
				iter++, iter2++)
			{
				Assert::AreEqual(*iter, *iter2);
			}

			Assert::AreEqual(default_value, column.default_value);

			std::string expected = "nishio VARCHAR(32) DEFAULT teacher PRIMARY KEY AUTO INCREMENT ";
			Assert::AreEqual(expected, column.to_string());
			
			Logger::WriteMessage(column.to_string().c_str());
		}

		TEST_METHOD(Column_CopyConstructor)
		{
			std::string column_name = "nishio";
			std::string type = "INT";
			std::list<std::string> options = { "PRIMARY KEY", "AUTO INCREMENT" };

			Db::Column column1(column_name, type, options);
			Db::Column column2(column1);

			Assert::AreEqual(column_name, column2.column_name);
			Assert::AreEqual(type, column2.data_type);


			std::list<std::string>::const_iterator iter2 = column2.options->begin();
			for (std::list<std::string>::const_iterator iter = options.begin();
			iter != options.end() && iter2 != column2.options->end();
				iter++, iter2++)
			{
				Assert::AreEqual(*iter, *iter2);
			}

			Logger::WriteMessage(column2.to_string().c_str());
		}

		TEST_METHOD(Column_operators)
		{
			std::string column_name1 = "nishio";
			std::string type1 = "INT";
			std::list<std::string> options1 = { "PRIMARY KEY", "AUTO INCREMENT" };
			Db::Column column1(column_name1, type1, options1);

			std::string column_name2 = "nishio";
			std::string type2 = "INT";
			std::list<std::string> options2 = { "AUTO INCREMENT", "PRIMARY KEY" };
			Db::Column column2(column_name2, type2, options2);

			std::string column_name3 = "nishio";
			std::string type = "INT";
			std::list<std::string> options3 = { "UNIQUE", "PRIMARY KEY" };
			Db::Column column3(column_name3, type, options3);

			Assert::IsTrue(column1 == column2);
			Assert::IsFalse(column2 == column3);
			Assert::IsFalse(column1 != column2);
			Assert::IsTrue(column2 != column3);
		}

	};
}
