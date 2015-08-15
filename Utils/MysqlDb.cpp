#include "stdafx.h"
#include "MySQLDb.h"


///<summary>
/// コンストラクタ
///</summary>
Db::MySQLDb::MySQLDb(std::unique_ptr<Db::IDbSettingsLoader> loader) : settings(std::make_unique<Db::DbSettings>(loader->load_db_settings()))
{
	sql::Driver* driver = get_driver_instance();
	std::string host = "tcp://" + settings->hostname + ":" + std::to_string(settings->port);
	connection = driver->connect(host, settings->username, settings->password);
}


///<summary>
/// コンストラクタ
///</summary>
Db::MySQLDb::MySQLDb(Db::DbSettings db_config) : settings(std::make_unique<Db::DbSettings>(db_config))
{
	sql::Driver* driver = get_driver_instance();
	std::string host = "tcp://" + settings->hostname + ":" + std::to_string(settings->port);
	connection = driver->connect(host, settings->username, settings->password);
}

///<summary>
/// デストラクタ
///</summary>
Db::MySQLDb::~MySQLDb()
{
	if (connection != nullptr) {
		if (!connection->isClosed()) connection->close();
		delete connection;
	}
}


///<summary>
/// コネクションチェックを挟んでクエリを実行
/// コネクションの接続が確立できない場合はnullptrを返す
///</summary>
template <typename RESULT_TYPE>
RESULT_TYPE* Db::MySQLDb::execute_if_connection_is_alive(const std::function<RESULT_TYPE(void)>& execute_function)
{
	if (connection != nullptr) {
		try {
			if (connection->isValid()) {
				RESULT_TYPE result = execute_function();
				return &result;
			}
			else {
				if (connection->reconnect()) {
					RESULT_TYPE result = execute_function();
					return &result;
				}
				return nullptr;
			}
		}
		catch (sql::SQLException &e) {
			std::cout << "# ERR: SQLException in " << __FILE__;
			std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
			std::cout << "# ERR: " << e.what();
			std::cout << " (MySQL error code: " << e.getErrorCode();
			std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
			return nullptr;
		}
	}
	return nullptr;
}


///<summary>
/// 使用するDBの選択，切り替えを行う
/// 成功時はtrueを失敗時はfalseを返す
///</summary>
bool Db::MySQLDb::use(const std::string& dbname)
{
	bool* result = execute_if_connection_is_alive<bool>([&] { 
		connection->setSchema(dbname);
		return true;
	});

	if (result != nullptr) return *result;
	return false;
}


///<summary>
/// CREATE TABLE等の結果を返さないクエリを実行します．
/// 成功時はtrueを失敗時はfalseを返す
///</summary>
bool Db::MySQLDb::execute(const std::string& query)
{
	bool* result =  execute_if_connection_is_alive<bool>([&] {
		sql::Statement* statement =  connection->createStatement();
		bool is_succeed = statement->execute(query);
		delete statement;
		return is_succeed;
	});
	
	if (result != nullptr) return *result;
	return false;
}


///<summary>
/// 文字列で渡されたクエリを実行し，その結果を返す
/// コネクションが無効な場合などは，nullptrを返します
/// 生のポインタを使っているので必ずdeleteすること
///</summary>
sql::ResultSet* Db::MySQLDb::row_query(const std::string& query)
{
	return *(execute_if_connection_is_alive<sql::ResultSet*>([&] {
		sql::Statement* statement = connection->createStatement();
		sql::ResultSet* result = statement->executeQuery(query);
		delete statement;
		return result;
	}));
}


///<summary>
/// データベース一覧を取得する
///</summary>
const std::list<std::string> Db::MySQLDb::get_databases()
{
	return *(execute_if_connection_is_alive<const std::list<std::string>>([&] {
		
		std::list<std::string> databases;
		sql::Statement* statement = connection->createStatement();
		sql::ResultSet* result = statement->executeQuery("SHOW DATABASES;");
		delete statement;

		result->beforeFirst();
		while (result->next()) {
			std::string dbname = result->getString(0);
			databases.push_back(dbname);
		}
		delete result;

		return databases;
	}));
}


///<summary>
/// 現在接続しているDB内のテーブル一覧を取得する
///</summary>
const std::list<std::string> Db::MySQLDb::get_tables()
{
	return *(execute_if_connection_is_alive<const std::list<std::string>>([&] {

		std::list<std::string> tables;
		sql::Statement* statement = connection->createStatement();
		sql::ResultSet* result = statement->executeQuery("SHOW TABLES;");
		delete statement;

		result->beforeFirst();
		while (result->next()) {
			std::string tablename = result->getString(0);
			tables.push_back(tablename);
		}
		delete result;

		return tables;
	}));
}


///<summary>
/// CREATE TABLEを実行するユーティリティ
///</summary>
bool Db::MySQLDb::create_table(std::unique_ptr<Db::BaseQueryGenerator> generator)
{
	const std::string query = generator->make_create_table_query();
	return row_query(query);
}


///<summary>
/// INSERTを実行するユーティリティ
///</summary>
bool Db::MySQLDb::insert(std::unique_ptr<Db::BaseQueryGenerator> generator, std::unordered_map<std::string, std::string> values)
{
	const std::string query = generator->make_insert_query();
	sql::PreparedStatement* statement = connection->prepareStatement(query);
	return true;
}