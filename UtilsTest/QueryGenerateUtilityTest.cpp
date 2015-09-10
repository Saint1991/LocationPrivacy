#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryGenerateUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(QueryGenerateUtilityTest)
	{
	public:

		class SerializableSample : public Serializable {
			const std::string to_string() const {
				return "Serialized";
			}
		};

		TEST_METHOD(QueryGenerateUtility_make_round_bracket_clause1)
		{
			std::vector<std::string> elements = { "a", "b" };
			std::string expected = "(a, b)";
			std::string actual = Db::QueryGenerateUtility::make_round_bracket_clause(elements);
			Assert::AreEqual(expected, actual);

			expected = "";
			elements.clear();
			Assert::AreEqual(expected, Db::QueryGenerateUtility::make_round_bracket_clause(elements));
		}

		TEST_METHOD(QueryGenerateUtility_make_round_bracket_clause2) 
		{
			std::shared_ptr<SerializableSample> sample1 = std::make_shared<SerializableSample>();
			std::shared_ptr<SerializableSample> sample2 = std::make_shared<SerializableSample>();
			std::shared_ptr<SerializableSample> sample3 = std::make_shared<SerializableSample>();
			std::vector<std::shared_ptr<Serializable>> sample_list = { sample1, sample2, sample3 };
			std::string actual = Db::QueryGenerateUtility::make_round_bracket_clause(sample_list);
			std::string expected = "(Serialized, Serialized, Serialized)";
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(QueryGenerateUtility_make_create_table_query)
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

			std::string actual = Db::QueryGenerateUtility::make_create_table_query(table);
			std::string expected = "CREATE TABLE lab_teachers (name VARCHAR(16) DEFAULT 'nanashi' , age INT NOT NULL , PRIMARY KEY (name)) CHARACTER SET UTF8;";
			Assert::AreEqual(expected, actual);

			table = Db::TableStructure(table_name, {});
			expected = "";
			Assert::AreEqual(expected, Db::QueryGenerateUtility::make_create_table_query(table));
		}

		TEST_METHOD(QueryGenerateUtility_make_insert_query1) 
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

			std::string actual = Db::QueryGenerateUtility::make_insert_query(table);
			std::string expected = "INSERT INTO lab_teachers (name, age) VALUES (?, ?)";
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(QueryGenerateUtility_make_insert_query2)
		{
			std::string table_name = "lab_teachers";
			std::vector<std::string> columns = { "name", "age" };
			std::string actual = Db::QueryGenerateUtility::make_insert_query(table_name, columns);
			std::string expected = "INSERT INTO lab_teachers (name, age) VALUES (?, ?)";
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(QueryGenerateUtility_make_select_query1)
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

			std::string actual = Db::QueryGenerateUtility::make_select_query(table);
			std::string expected = "SELECT name, age FROM lab_teachers";
			Assert::AreEqual(expected, actual);

			actual = Db::QueryGenerateUtility::make_select_query(table, "name='hara'");
			expected = "SELECT name, age FROM lab_teachers WHERE name='hara'";
			Assert::AreEqual(expected, actual);

			Db::TableStructure table2("lab_teachers", {});
			actual = Db::QueryGenerateUtility::make_select_query(table2);
			expected = "";
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(QueryGenerateUtility_make_select_query2)
		{
			std::string table_name = "lab_teachers";
			std::string actual = Db::QueryGenerateUtility::make_select_query(table_name, { "age" });
			std::string expected = "SELECT age FROM lab_teachers";
			Assert::AreEqual(expected, actual);

			actual = Db::QueryGenerateUtility::make_select_query(table_name, { "name" }, "name='hara'");
			expected = "SELECT name FROM lab_teachers WHERE name='hara'";
			Assert::AreEqual(expected, actual);

			expected = "";
			Assert::AreEqual(expected, Db::QueryGenerateUtility::make_select_query(table_name, {}));
		}

		TEST_METHOD(QueryGenerateUtility_make_update_query)
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

			std::string actual = Db::QueryGenerateUtility::make_update_query(table);
			std::string expected = "UPDATE lab_teachers SET name=?, age=?";
			Assert::AreEqual(expected, actual);

			actual = Db::QueryGenerateUtility::make_update_query(table, "age=42");
			expected = "UPDATE lab_teachers SET name=?, age=? WHERE age=42";
			Assert::AreEqual(expected, actual);

			Db::TableStructure table2(table_name, {});
			actual = Db::QueryGenerateUtility::make_update_query(table2);
			expected = "";
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(QueryGenerateUtility_make_update_query2)
		{
			std::string table_name = "lab_teachers";
			std::string actual = Db::QueryGenerateUtility::make_update_query(table_name, { "age" });
			std::string expected = "UPDATE lab_teachers SET age=?";
			Assert::AreEqual(expected, actual);

			actual = Db::QueryGenerateUtility::make_update_query(table_name, { "name" }, "name='hara'");
			expected = "UPDATE lab_teachers SET name=? WHERE name='hara'";
			Assert::AreEqual(expected, actual);

			actual = Db::QueryGenerateUtility::make_update_query(table_name, {});
			expected = "";
			Assert::AreEqual(expected, actual);
		}
	};
}
