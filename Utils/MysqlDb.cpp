#include "stdafx.h"
#include "MySQLDb.h"


namespace Db
{
	///<summary>
	/// コンストラクタ
	///</summary>
	 MySQLDb::MySQLDb(std::unique_ptr< IDbSettingsLoader> loader) : settings(std::make_unique<DbSettings>(loader->load_db_settings()))
	{
		sql::Driver* driver = get_driver_instance();
		std::string host = "tcp://" + settings->hostname + ":" + std::to_string(settings->port);
		connection = driver->connect(host, settings->username, settings->password);
	}


	///<summary>
	/// コンストラクタ
	///</summary>
	 MySQLDb::MySQLDb(DbSettings db_config) : settings(std::make_unique<DbSettings>(db_config))
	{
		sql::Driver* driver = get_driver_instance();
		std::string host = "tcp://" + settings->hostname + ":" + std::to_string(settings->port);
		connection = driver->connect(host, settings->username, settings->password);
	}

	///<summary>
	/// デストラクタ
	///</summary>
	 MySQLDb::~MySQLDb()
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
	RESULT_TYPE*  MySQLDb::execute_if_connection_is_alive(const std::function<RESULT_TYPE*(void)>& execute_function)
	{
		if (connection != nullptr) {
			try {
				if (connection->isValid()) {
					RESULT_TYPE* result = execute_function();
					return std::move(result);
				}
				else {
					if (connection->reconnect()) {
						RESULT_TYPE* result = execute_function();
						return std::move(result);
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

	/// 上記の明示的特殊化
	/// 意味的には上のメソッドのRESULT_TYPEにbool, ResultSet, intをそれぞれあてはめた3つのメソッドを定義したのと同義
	template bool*  MySQLDb::execute_if_connection_is_alive(const std::function<bool*(void)>& execute_function);
	template sql::ResultSet*  MySQLDb::execute_if_connection_is_alive(const std::function<sql::ResultSet*(void)>& execute_function);
	template int*  MySQLDb::execute_if_connection_is_alive(const std::function<int*(void)>& execute_function);


	///<summary>
	/// 使用するDBの選択，切り替えを行う
	/// 成功時はtrueを失敗時はfalseを返す
	///</summary>
	bool  MySQLDb::use(const std::string& dbname)
	{
		bool* result = execute_if_connection_is_alive<bool>([&] {
			connection->setSchema(dbname);
			bool ret = true;
			return std::move(&ret);
		});

		if (result == nullptr) return false;
		return *result;
	}


	///<summary>
	/// CREATE TABLE等の結果を返さないクエリを実行します．
	/// 成功時はtrueを失敗時はfalseを返す
	///</summary>
	bool  MySQLDb::execute(const std::string& query)
	{
		bool* result = execute_if_connection_is_alive<bool>([&] {
			sql::Statement* statement = connection->createStatement();
			bool is_succeed = statement->execute(query);
			delete statement;
			return std::move(&is_succeed);
		});

		if (result == nullptr) return false;
		return *result;
	}


	///<summary>
	/// 文字列で渡されたクエリを実行し，その結果を返す
	/// コネクションが無効な場合などは，nullptrを返します
	/// 生のポインタを使っているので必ずdeleteすること
	///</summary>
	sql::ResultSet*  MySQLDb::raw_query(const std::string& query)
	{
		sql::ResultSet* ret = execute_if_connection_is_alive<sql::ResultSet>([&] {
			sql::Statement* statement = connection->createStatement();
			sql::ResultSet* result = statement->executeQuery(query);
			delete statement;
			return std::move(result);
		});

		return std::move(ret);
	}


	///<summary>
	/// データベース一覧を取得する
	/// 失敗した場合も空のリストが返るので注意
	///</summary>
	const std::list<std::string>  MySQLDb::get_databases()
	{
		sql::ResultSet* result_set = raw_query("SHOW DATABASES");
		if (result_set == nullptr) return std::list<std::string>();

		std::list<std::string> ret;
		result_set->beforeFirst();
		while (result_set->next()) {
			std::string dbname = result_set->getString(1);
			ret.push_back(dbname);
		}
		delete result_set;
		return ret;
	}


	///<summary>
	/// 現在接続しているDB内のテーブル一覧を取得する
	/// 失敗した場合も空のリストが返るので注意
	///</summary>
	const std::list<std::string>  MySQLDb::get_tables()
	{
		sql::ResultSet* result_set = raw_query("SHOW TABLES");
		if (result_set == nullptr) return std::list<std::string>();

		std::list<std::string> ret;
		result_set->beforeFirst();
		while (result_set->next()) {
			std::string table_name = result_set->getString(1);
			ret.push_back(table_name);
		}
		delete result_set;
		return ret;
	}


	///<summary>
	/// queryをコンパイルしてPreparedStatementを取得する
	///</summary>
	sql::PreparedStatement* MySQLDb::prepare(const std::string& query)
	{
		sql::PreparedStatement* ret = nullptr;
		
		if (!connection->isValid()) {
			if (connection->reconnect()) ret = connection->prepareStatement(query);
		}
		else ret = connection->prepareStatement(query);
		return ret;
	}

	///<summary>
	/// CREATE TABLEを実行するユーティリティ
	/// 失敗した場合はfalseを返す
	///</summary>
	bool  MySQLDb::create_table(const  TableStructure& table_info)
	{
		const std::string query =  QueryGenerateUtility::make_create_table_query(table_info);
		return execute(query);
	}


	///<summary>
	/// INSERTを実行するユーティリティ
	/// 失敗した場合はfalseを返す
	///</summary>
	bool  MySQLDb::insert(const std::string& table_name, const std::vector<std::string>& columns, std::shared_ptr<Bindable const> data)
	{
		bool* ret = execute_if_connection_is_alive<bool>([&]() {
			const std::string query =  QueryGenerateUtility::make_insert_query(table_name, columns);
			sql::PreparedStatement* statement = connection->prepareStatement(query);
			data->bind(statement, columns);
			bool result = statement->execute();
			delete statement;
			return std::move(&result);
		});
		if (ret == nullptr) return false;
		return *ret;
	}


	///<summary>
	/// INSERTを実行するユーティリティ
	/// 失敗した場合はfalseを返す
	///</summary>
	bool  MySQLDb::insert(const TableStructure& insert_columns, std::shared_ptr<Bindable const> data)
	{
		return insert(insert_columns.table_name, insert_columns.get_column_names(), data);
	}


	///<summary>
	/// INSERTを実行するユーティリティ
	/// 複数データをインサートする場合はこちらを使うと効率がよい
	/// 失敗した場合はfalseを返す
	///</summary>
	bool  MySQLDb::insert(const std::string& table_name, const std::vector<std::string>& columns, const std::list<std::shared_ptr<Bindable const>>& data_list)
	{
		bool* result = execute_if_connection_is_alive<bool>([&]() {
			const std::string query =  QueryGenerateUtility::make_insert_query(table_name, columns);
			sql::PreparedStatement* statement = connection->prepareStatement(query);

			bool are_all_succeed = true;
			for (std::list<std::shared_ptr<Bindable const>>::const_iterator iter = data_list.begin(); iter != data_list.end(); iter++) {
				(*iter)->bind(statement, columns);
				are_all_succeed && statement->execute();
			}
			delete statement;
			return std::move(&are_all_succeed);
		});

		if (result == nullptr) return false;
		return *result;
	}


	///<summary>
	/// INSERTを実行するユーティリティ
	/// 複数データをインサートする場合はこちらを使うと効率がよい
	/// 失敗した場合はfalseを返す
	///</summary>
	bool  MySQLDb::insert(const  TableStructure& insert_columns, const std::list<std::shared_ptr<Bindable const>>& data_list)
	{
		return insert(insert_columns.table_name, insert_columns.get_column_names(), data_list);
	}


	///<summary>
	/// UPDATEを実行するユーティリティ
	/// 変更されたカラム数を返す
	/// 失敗した場合は-1を返す
	///</summary>
	int  MySQLDb::update(const std::string& table_name, const std::vector<std::string>& columns, std::shared_ptr<Bindable const> data, const std::string& where_clause)
	{
		int* ret = execute_if_connection_is_alive<int>([&]() {
			const std::string query =  QueryGenerateUtility::make_update_query(table_name, columns, where_clause);
			sql::PreparedStatement* statement = connection->prepareStatement(query);
			data->bind(statement, columns);
			int result = statement->executeUpdate();
			delete statement;
			return std::move(&result);
		});

		if (ret == nullptr) return -1;
		return *ret;
	}


	///<summary>
	/// UPDATEを実行するユーティリティ
	/// 変更されたカラム数を返す
	/// 失敗した場合は-1を返す
	///</summary>
	int  MySQLDb::update(const  TableStructure& update_columns, std::shared_ptr<Bindable const> data, std::string where_clause)
	{
		return update(update_columns.table_name, update_columns.get_column_names(), data, where_clause);
	}



	///<summary>
	/// 簡単なSELECTクエリを実行するユーティリティ
	/// 生のポインタを用いているのでdeleteを忘れず行うこと!
	///</summary>
	sql::ResultSet*  MySQLDb::select(const std::string& table_name, const std::vector<std::string>& columns, const std::string& where_clause)
	{
		const std::string query =  QueryGenerateUtility::make_select_query(table_name, columns, where_clause);
		return std::move(raw_query(query));
	}

	///<summary>
	/// 簡単なSELECTクエリを実行するユーティリティ
	/// 生のポインタを用いているのでdeleteを忘れず行うこと!
	///</summary>
	sql::ResultSet*  MySQLDb::select(const  TableStructure& select_columns, const std::string where_clause)
	{
		return std::move(select(select_columns.table_name, select_columns.get_column_names(), where_clause));
	}
}

