#include "stdafx.h"
#include "CppUnitTest.h"
#include "MySQLDb.h"
#include "IDbSettingsLoader.h"
#include "DbSettingsFileLoader.h"
#include "Bindable.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Db;

namespace UtilsTest
{
	//DBの内容に依存するので，動作確認に留め，Assertはしない
	TEST_CLASS(MySQLDbTest)
	{
	public:
		const std::string SETTING_FILE_PATH = "../settings/mydbsettings.xml";

		struct BindableSample : public Bindable {
			int id;
			std::string name;

			BindableSample(int id, std::string name) : id(id), name(name) {}

			void bind(sql::PreparedStatement* statement, const Db::TableStructure& table_info) const 
			{
				int index = table_info.get_column_index("id");
				if (index != 0) statement->setInt(index, this->id);

				index = table_info.get_column_index("name");
				if (index != 0) statement->setString(index, name);
			}

			void bind(sql::PreparedStatement* statement, const std::vector<std::string>& columns) const 
			{
				int index = std::find(columns.begin(), columns.end(), "id") - columns.begin() + 1;
				if (index <= columns.size()) statement->setInt(index, this->id);
				
				index = std::find(columns.begin(), columns.end(), "name") - columns.begin() + 1;
				if (index <= columns.size()) statement->setString(index, name);
			}
		};

		//動作確認OK
		TEST_METHOD(MySQLDb1)
		{
			std::unique_ptr<IDbSettingsLoader> loader = std::make_unique<DbSettingsFileLoader>(SETTING_FILE_PATH);
			MySQLDb db(std::move(loader));
			
			//database一覧の取得
			const std::list<std::string> dbnames = db.get_databases();

			//testというdatabaseに移動，移動できなければ終了
			if (!db.use("test")) return;

			//testの中のテーブル一覧を取得
			std::list<std::string> table_names = db.get_tables();

			//testの中にhogehogeというテーブルを作成
			db.execute("CREATE TABLE IF NOT EXISTS hogehoge (id INT PRIMARY KEY, name VARCHAR(32) NOT NULL) CHARACTER SET UTF8;");
		}

		//動作確認OK
		TEST_METHOD(MySQLDb2)
		{
			std::unique_ptr<IDbSettingsLoader> loader = std::make_unique<DbSettingsFileLoader>(SETTING_FILE_PATH);
			MySQLDb db(std::move(loader));

			db.use("test");

			//Tableを空にする
			db.execute("TRUNCATE hogehoge;");
			
			//データの単発挿入
			std::shared_ptr<Bindable const> insert_data = std::make_shared<BindableSample const>(1, "aiu");
		
			db.insert("hogehoge", { "id" , "name" }, insert_data);
			
			//複数データの一括挿入
			std::list<std::shared_ptr<Bindable const>> insert_datas = {
				{std::make_shared<BindableSample const>(2, "eo")},
				{std::make_shared<BindableSample const>(3, "kakiku")},
				{std::make_shared<BindableSample const>(4, "keko")}
			};
			db.insert("hogehoge", { "name", "id" }, insert_datas);

			//ID４のデータの名前を変更してみる
			db.update("hogehoge", { "name" }, std::make_shared<BindableSample const>(1, "sasiko"), "id=4");

			sql::ResultSet* result = db.select("hogehoge", { "id", "name" }, "id > 2");
			if (result == nullptr) return;
			std::list<BindableSample> selected_samples;
			result->beforeFirst();
			while (result->next()) {
				int id = result->getInt("id");
				std::string name = result->getString("name");
				selected_samples.push_back(BindableSample(id, name));
			}
		}
	};
}
