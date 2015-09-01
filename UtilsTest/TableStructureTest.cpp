
#include "stdafx.h"
#include "CppUnitTest.h"
#include "TableStructure.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(TableStructureTest)
	{
	public:

		TEST_METHOD(TableStructure_Constructors)
		{
			std::string column_name1 = "nishio";
			std::string type1 = "INT";
			std::list<std::string> options1 = { "PRIMARY KEY", "AUTO INCREMENT" };
			Db::Column column1(column_name1, type1, options1);
			
			std::string column_name2 = "hara";
			std::string type2 = "DOUBLE";
			std::list<std::string> options2 = { "PRIMARY KEY", "AUTO INCREMENT" };
			Db::Column column2(column_name2, type2, options2);


			std::string table_name = "lab_teacher";
			std::list<Db::Column> column = { column1,column2 };
			Db::TableStructure table1(table_name, column);
			Db::TableStructure table2(table1);

			Assert::AreEqual(table_name, table1.table_name);
			Assert::AreEqual(table_name, table2.table_name);

			
			std::list<Db::Column>::const_iterator iter2 = table1.columns->begin();
			for (std::list<Db::Column>::const_iterator iter = column.begin();
				 iter != column.end() && iter2 != table1.columns->end();
				 iter++, iter2++)
			{
				Assert::IsTrue(*iter == *iter2);
			}

			std::list<Db::Column>::const_iterator iter3 = table2.columns->begin();
			for (std::list<Db::Column>::const_iterator iter = column.begin();
				 iter != column.end() && iter3 != table2.columns->end();
				 iter++, iter3++)
			{
				Assert::IsTrue(*iter == *iter3);
			}
			
			
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


			std::string table_name = "lab_teacher";
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


			std::string table_name = "lab_teacher";
			std::list<Db::Column> column = { column1,column2 };
			Db::TableStructure table(table_name, column);

			std::list<std::string> name_list = table.get_column_name_list();
			std::list<std::string>::const_iterator iter = name_list.begin();
			
			Assert::AreEqual(1U, table.get_column_index(*iter));

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


			std::string table_name = "lab_teacher";
			std::list<Db::Column> column = { column1,column2 };
			Db::TableStructure table(table_name, column);

			std::list<std::string> name_list = table.get_column_name_list();

			for (std::list<std::string>::const_iterator iter = name_list.begin();
			     iter != name_list.end();
				 iter++)
			{
				Logger::WriteMessage(iter->c_str());
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


			std::string table_name = "lab_teacher";
			std::list<Db::Column> column = { column1,column2 };
			Db::TableStructure table(table_name, column);


			if (table.add_primary_key(column_name1) == false) {
				Assert::Fail();
			}

		}
		
	};
}
