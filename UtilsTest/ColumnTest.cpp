
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Column.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(ColumnTest)
	{
	public:

		TEST_METHOD(Column_Constructor1)
		{
			std::string column_name = "nishio";
			std::string type = "INT";
			std::list<std::string> options = { "PRIMARY KEY", "AUTO INCREMENT" };
			Db::Column column(column_name, type, options);
			
			Assert::AreEqual(column_name, column.column_name);
			Assert::AreEqual(type, column.data_type);


			std::list<std::string>::const_iterator iter2 = column.options->begin();
			for (std::list<std::string>::const_iterator iter = options.begin();
				 iter != options.end() && iter2 != column.options->end();
				 iter++, iter2++) 
			{
				Assert::AreEqual(*iter, *iter2);
			}

			Logger::WriteMessage(column.to_string().c_str());
		}
		
		TEST_METHOD(Column_Constructor2)
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
		
	};
}
