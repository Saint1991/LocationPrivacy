
#include "stdafx.h"
#include "CppUnitTest.h"
#include "TableStructure.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(TableStructureTest)
	{
	public:

		TEST_METHOD(TableStructure_Constructor)
		{
			std::string column_name1 = "nishio";
			std::string type1 = "INT";
			std::list<std::string> options1 = { "PRIMARY KEY", "AUTO INCREMENT" };
			Db::Column column1(column_name1, type1, options1);

			std::string column_name2 = "hara";
			std::string type2 = "DOUBLE";
			std::list<std::string> options2 = { "PRIMARY KEY", "AUTO INCREMENT" };
			Db::Column column2(column_name2, type2, options2);


			std::string table_name = "lab_teachers";
			std::list<Db::Column> column = { column1,column2 };
			Db::TableStructure table(table_name, column);

			Assert::IsTrue(table.primary_keys == nullptr);
			Assert::AreEqual(table_name, table.table_name);


			std::list<Db::Column>::const_iterator iter2 = table.columns->begin();
			for (std::list<Db::Column>::const_iterator iter = column.begin();
			iter != column.end() && iter2 != table.columns->end();
				iter++, iter2++)
			{
				Assert::IsTrue(*iter == *iter2);
			}

			

		}

		TEST_METHOD(TableStructure_CopyConstructor)
		{
			std::string column_name1 = "nishio";
			std::string type1 = "VARCHAR(32)";
			std::list<std::string> options1 = { "PRIMARY KEY", "AUTO INCREMENT" };
			std::string default_value1 = "teacher";
			Db::Column column1(column_name1, type1, options1, default_value1);

			std::string column_name2 = "hara";
			std::string type2 = "VARCHAR(32)";
			std::list<std::string> options2 = { "PRIMARY KEY", "AUTO INCREMENT" };
			std::string default_value2 = "teacher";
			Db::Column column2(column_name2, type2, options2, default_value2);


			std::string table_name = "lab_teachers";
			std::list<Db::Column> column = { column1,column2 };
			Db::TableStructure table1(table_name, column);
			Db::TableStructure table2(table1);

			std::list<Db::Column>::const_iterator iter2 = table2.columns->begin();
			for (std::list<Db::Column>::const_iterator iter = column.begin();
			iter != column.end() && iter2 != table2.columns->end();
				iter++, iter2++)
			{
				Assert::IsTrue(*iter == *iter2);
			}

			table2.columns->begin()->options->push_back("ALTERNATE KEY");
			Logger::WriteMessage(table1.columns->begin()->to_string().c_str());
			Logger::WriteMessage(table2.columns->begin()->to_string().c_str());

		}

		TEST_METHOD(TableStructure_is_column_exists)
		{

			std::string column_name1 = "nishio";
			std::string type1 = "INT";
			std::list<std::string> options1 = { "PRIMARY KEY", "AUTO INCREMENT" };
			Db::Column column1(column_name1, type1, options1);

			std::string column_name2 = "hara";
			std::string type2 = "DOUBLE";
			std::list<std::string> options2 = { "PRIMARY KEY", "AUTO INCREMENT" };
			Db::Column column2(column_name2, type2, options2);


			std::string table_name = "lab_teachers";
			std::list<Db::Column> column = { column1,column2 };
			Db::TableStructure table(table_name, column);


			Assert::IsTrue(table.is_column_exists("nishio"));
			Assert::IsFalse(table.is_column_exists("lab"));

		}

		TEST_METHOD(TableStructure_get_column_index)
		{
			std::string column_name1 = "nishio";
			std::string type1 = "INT";
			std::list<std::string> options1 = { "PRIMARY KEY", "AUTO INCREMENT" };
			Db::Column column1(column_name1, type1, options1);

			std::string column_name2 = "hara";
			std::string type2 = "DOUBLE";
			std::list<std::string> options2 = { "PRIMARY KEY", "AUTO INCREMENT" };
			Db::Column column2(column_name2, type2, options2);


			std::string table_name = "lab_teachers";
			std::list<Db::Column> column = { column1,column2 };
			Db::TableStructure table(table_name, column);

			
			Assert::AreEqual(0U, table.get_column_index("lab"));
			Assert::AreEqual(1U, table.get_column_index(column1.column_name));
			Assert::AreEqual(2U, table.get_column_index(column2.column_name));


		}

		TEST_METHOD(TableStructure_get_column_name)
		{
			std::string column_name1 = "nishio";
			std::string type1 = "INT";
			std::list<std::string> options1 = { "PRIMARY KEY", "AUTO INCREMENT" };
			Db::Column column1(column_name1, type1, options1);

			std::string column_name2 = "hara";
			std::string type2 = "DOUBLE";
			std::list<std::string> options2 = { "PRIMARY KEY", "AUTO INCREMENT" };
			Db::Column column2(column_name2, type2, options2);


			std::string table_name = "lab_teachers";
			std::list<Db::Column> column = { column1,column2 };
			Db::TableStructure table(table_name, column);

			std::list<std::string> name_list = table.get_column_name_list();

			for (std::list<std::string>::iterator iter = name_list.begin();
				iter->c_str() == "nishio"; iter++)
			{
				Assert::AreEqual("hara", iter->c_str());
			}
		}

		TEST_METHOD(TableStructure_add_primary_key)
		{
			std::string column_name1 = "nishio";
			std::string type1 = "INT";
			std::list<std::string> options1 = { "PRIMARY KEY", "AUTO INCREMENT" };
			Db::Column column1(column_name1, type1, options1);

			std::string column_name2 = "hara";
			std::string type2 = "DOUBLE";
			std::list<std::string> options2 = { "PRIMARY KEY", "AUTO INCREMENT" };
			Db::Column column2(column_name2, type2, options2);


			std::string table_name = "lab_teachers";
			std::list<Db::Column> column = { column1,column2 };
			Db::TableStructure table(table_name, column);


			Assert::IsFalse(table.add_primary_key("lab"));
			Assert::IsFalse(table.add_primary_key("nisio"));

		}

	};
}