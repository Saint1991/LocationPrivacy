
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
			std::string column_name1 = "name";
			std::string type1 = "VARCHAR(16)";
			std::list<std::string> options1 = { "PRIMARY KEY" };
			Db::Column column1(column_name1, type1, options1);

			std::string column_name2 = "age";
			std::string type2 = "INT";
			std::list<std::string> options2 = { "PRIMARY KEY", "NOT NULL" };
			Db::Column column2(column_name2, type2, options2);

			std::string table_name = "lab_teachers";
			std::list<Db::Column> columns = { column1,column2 };
			Db::TableStructure table(table_name, columns);

			Assert::IsTrue(table.table_name == "lab_teachers");
			
			std::list<std::string>::const_iterator primary_key = table.get_primary_keys()->begin();
			Assert::IsTrue(*primary_key++ == "name");
			Assert::IsTrue(*primary_key == "age");

			//optionsからPRIMARY KEYは消えているはず
			std::list<Db::Column>::const_iterator column = table.get_column_list()->begin();
			column1.options = std::make_unique<std::list<std::string>>();
			column2.options = std::make_unique<std::list<std::string>>(1, "NOT NULL");
			Assert::IsTrue(column1 == *column++);
			Assert::IsTrue(column2 == *column);

			Logger::WriteMessage(table.to_string().c_str());


			//こっからはPRIMARY KEY設定がない場合にnullptrかどうか
			std::string column_name3 = "name";
			std::string type3 = "VARCHAR(16)";
			std::list<std::string> options3 = { "NOT NULL" };
			Db::Column column3(column_name3, type3, options3);
			Db::TableStructure table2(table_name, {column3});
			Assert::IsTrue(table2.get_primary_keys() == nullptr);
			
			Logger::WriteMessage(table2.to_string().c_str());
		}

		TEST_METHOD(TableStructure_CopyConstructor)
		{
			std::string column_name1 = "name";
			std::string type1 = "VARCHAR(16)";
			std::list<std::string> options1 = { "PRIMARY KEY" };
			Db::Column column1(column_name1, type1, options1);

			std::string table_name = "lab_teachers";
			Db::TableStructure table(table_name, {column1});

			//コピーコンストラクタ
			Db::TableStructure table2(table);
			std::list<Db::Column>::const_iterator column = table2.get_column_list()->begin();
			std::list<std::string>::const_iterator primary_key = table2.get_primary_keys()->begin();
			Assert::AreEqual(std::string("lab_teachers"), table2.table_name);
			column1.options = std::make_unique<std::list<std::string>>();
			Assert::IsTrue(column1 == *column);
			Assert::AreEqual(std::string("name"), *primary_key);

			//コピー後のポインタ周りのチェック
			std::string column_name2 = "age";
			std::string type2 = "INT";
			std::list<std::string> options2 = { "PRIMARY KEY", "NOT NULL" };
			Db::Column column2(column_name2, type2, options2);
			Assert::IsTrue(table2.add_column(column2));

			Assert::AreEqual(1U, table.get_primary_keys()->size());
			Assert::AreEqual(1U, table.get_column_list()->size());
			Assert::AreEqual(2U, table2.get_primary_keys()->size());
			Assert::AreEqual(2U, table2.get_column_list()->size());

			Logger::WriteMessage(table.to_string().c_str());
			Logger::WriteMessage(table2.to_string().c_str());
		}

		TEST_METHOD(TableStructure_is_column_exists)
		{
			std::string column_name1 = "name";
			std::string type1 = "VARCHAR(16)";
			std::list<std::string> options1 = { "PRIMARY KEY" };
			Db::Column column1(column_name1, type1, options1);

			std::string table_name = "lab_teachers";
			Db::TableStructure table(table_name, { column1 });

			Assert::IsTrue(table.is_column_exists("name"));
			Assert::IsFalse(table.is_column_exists("gender"));
		}

		TEST_METHOD(TableStructure_get_column_index)
		{
			std::string column_name1 = "name";
			std::string type1 = "VARCHAR(16)";
			std::list<std::string> options1 = { "PRIMARY KEY" };
			Db::Column column1(column_name1, type1, options1);

			std::string table_name = "lab_teachers";
			Db::TableStructure table(table_name, { column1 });

			Assert::AreEqual(1U, table.get_column_index("name"));
			Assert::AreEqual(0U, table.get_column_index("gender"));
		}

		TEST_METHOD(TableStructure_get_column_name_list)
		{
			std::string column_name1 = "name";
			std::string type1 = "VARCHAR(16)";
			std::list<std::string> options1 = { "PRIMARY KEY" };
			Db::Column column1(column_name1, type1, options1);

			std::string column_name2 = "age";
			std::string type2 = "INT";
			std::list<std::string> options2 = { "PRIMARY KEY", "NOT NULL" };
			Db::Column column2(column_name2, type2, options2);

			std::string table_name = "lab_teachers";
			std::list<Db::Column> columns = { column1,column2 };
			Db::TableStructure table(table_name, columns);

			std::list<std::string> name_list = table.get_column_name_list();
			std::list<std::string>::const_iterator iter = name_list.begin();
			Assert::AreEqual(std::string("name"), *iter++);
			Assert::AreEqual(std::string("age"), *iter);
		}

		TEST_METHOD(TableStructure_add_primary_key)
		{
			std::string column_name1 = "name";
			std::string type1 = "VARCHAR(16)";
			std::list<std::string> options1 = { "PRIMARY KEY" };
			Db::Column column1(column_name1, type1, options1);

			std::string column_name2 = "age";
			std::string type2 = "INT";
			std::list<std::string> options2 = { "NOT NULL" };
			Db::Column column2(column_name2, type2, options2);

			std::string table_name = "lab_teachers";
			std::list<Db::Column> columns = { column1,column2 };
			Db::TableStructure table(table_name, columns);

			//Tableにないカラムの場合false
			Assert::IsFalse(table.add_primary_key(std::string("gender")));

			//既にPRIMARY KEYの場合もfalse
			Assert::IsFalse(table.add_primary_key(std::string("name")));

			//ここまでではTableのPrimaryKeysはname1つのはず
			Assert::AreEqual(1U, table.get_primary_keys()->size());
			std::list<std::string>::const_iterator iter = table.get_primary_keys()->begin();
			Assert::AreEqual(std::string("name"), *iter);

			//ageをPRIMARY KEYに追加してみる
			Assert::IsTrue(table.add_primary_key("age"));
			Assert::AreEqual(2U, table.get_primary_keys()->size());
		}

		TEST_METHOD(TableStructure_add_column) 
		{
			std::string column_name1 = "name";
			std::string type1 = "VARCHAR(16)";
			std::list<std::string> options1 = { "PRIMARY KEY" };
			Db::Column column1(column_name1, type1, options1);

			std::string table_name = "lab_teachers";
			Db::TableStructure table(table_name, {});

			Assert::IsTrue(table.add_column(column1));
			auto column_list = table.get_column_list();
			Assert::AreEqual(1U, column_list->size());
			Db::Column column = *column_list->begin();
			Assert::AreEqual(std::string("name"), column.column_name);
			Assert::AreEqual(std::string("VARCHAR(16)"), column.data_type);
			Assert::AreEqual(0U, column.default_value.length());
			Assert::AreEqual(0U, column.options->size());
			
			Assert::IsFalse(table.add_column(column1));
			column_list = table.get_column_list();
			Assert::AreEqual(1U, column_list->size());
			Assert::IsTrue(column1 == *column_list->begin());
		}

		TEST_METHOD(TableStructure_get_column_list) 
		{
			std::string column_name1 = "name";
			std::string type1 = "VARCHAR(16)";
			std::list<std::string> options1 = { "UNIQUE" };
			Db::Column column1(column_name1, type1, options1);

			std::string table_name = "lab_teachers";
			Db::TableStructure table(table_name, {column1});

			std::shared_ptr<const std::list<Db::Column>> column_list = table.get_column_list();
			Db::Column column = *column_list->begin();
			Assert::IsTrue(column1 == column);

			column.column_name = "age";
			column_list = table.get_column_list();
			Assert::AreEqual(std::string("name"), column_list->begin()->column_name);
		}

		TEST_METHOD(TableStructure_get_primary_keys) 
		{
			std::string column_name1 = "name";
			std::string type1 = "VARCHAR(16)";
			std::list<std::string> options1 = { "PRIMARY KEY" };
			Db::Column column1(column_name1, type1, options1);

			std::string table_name = "lab_teachers";
			Db::TableStructure table(table_name, { column1 });

			std::shared_ptr<const std::list<std::string>> primary_keys = table.get_primary_keys();
			Assert::AreEqual(std::string("name"), *primary_keys->begin());

			std::string key_name = *primary_keys->begin();
			key_name = "abc";

			Assert::AreEqual(std::string("name"), *table.get_primary_keys()->begin());
		}

		TEST_METHOD(TableStructure_to_string) 
		{
			std::string column_name1 = "name";
			std::string type1 = "VARCHAR(16)";
			std::list<std::string> options1 = { "PRIMARY KEY" };
			Db::Column column1(column_name1, type1, options1, "nanashi");

			std::string column_name2 = "age";
			std::string type2 = "INT";
			std::list<std::string> options2 = { "NOT NULL" };
			Db::Column column2(column_name2, type2, options2);

			std::string table_name = "lab_teachers";
			std::list<Db::Column> columns = { column1,column2 };
			Db::TableStructure table(table_name, columns);

			std::string actual = table.to_string();
			std::string expected = "CREATE TABLE lab_teachers (name VARCHAR(16) DEFAULT 'nanashi' , age INT NOT NULL , PRIMARY KEY (name)) CHARACTER SET UTF8;";
			Logger::WriteMessage(actual.c_str());
			Assert::AreEqual(expected, actual);
		}
	};
}