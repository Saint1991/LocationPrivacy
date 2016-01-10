#include "stdafx.h"
#include "MySQLDb.h"


namespace Db
{
	///<summary>
	/// �R���X�g���N�^
	///</summary>
	 MySQLDb::MySQLDb(std::unique_ptr< IDbSettingsLoader> loader) : settings(std::make_unique<DbSettings>(loader->load_db_settings()))
	{
		sql::Driver* driver = get_driver_instance();
		std::string host = "tcp://" + settings->hostname + ":" + std::to_string(settings->port);
		connection = driver->connect(host, settings->username, settings->password);
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	 MySQLDb::MySQLDb(DbSettings db_config) : settings(std::make_unique<DbSettings>(db_config))
	{
		sql::Driver* driver = get_driver_instance();
		std::string host = "tcp://" + settings->hostname + ":" + std::to_string(settings->port);
		connection = driver->connect(host, settings->username, settings->password);
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	 MySQLDb::~MySQLDb()
	{
		if (connection != nullptr) {
			if (!connection->isClosed()) connection->close();
			delete connection;
		}
	}


	///<summary>
	/// �R�l�N�V�����`�F�b�N������ŃN�G�������s
	/// �R�l�N�V�����̐ڑ����m���ł��Ȃ��ꍇ��nullptr��Ԃ�
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

	/// ��L�̖����I���ꉻ
	/// �Ӗ��I�ɂ͏�̃��\�b�h��RESULT_TYPE��bool, ResultSet, int�����ꂼ�ꂠ�Ă͂߂�3�̃��\�b�h���`�����̂Ɠ��`
	template bool*  MySQLDb::execute_if_connection_is_alive(const std::function<bool*(void)>& execute_function);
	template sql::ResultSet*  MySQLDb::execute_if_connection_is_alive(const std::function<sql::ResultSet*(void)>& execute_function);
	template int*  MySQLDb::execute_if_connection_is_alive(const std::function<int*(void)>& execute_function);


	///<summary>
	/// �g�p����DB�̑I���C�؂�ւ����s��
	/// ��������true�����s����false��Ԃ�
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
	/// CREATE TABLE���̌��ʂ�Ԃ��Ȃ��N�G�������s���܂��D
	/// ��������true�����s����false��Ԃ�
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
	/// ������œn���ꂽ�N�G�������s���C���̌��ʂ�Ԃ�
	/// �R�l�N�V�����������ȏꍇ�Ȃǂ́Cnullptr��Ԃ��܂�
	/// ���̃|�C���^���g���Ă���̂ŕK��delete���邱��
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
	/// �f�[�^�x�[�X�ꗗ���擾����
	/// ���s�����ꍇ����̃��X�g���Ԃ�̂Œ���
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
	/// ���ݐڑ����Ă���DB���̃e�[�u���ꗗ���擾����
	/// ���s�����ꍇ����̃��X�g���Ԃ�̂Œ���
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
	/// query���R���p�C������PreparedStatement���擾����
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
	/// CREATE TABLE�����s���郆�[�e�B���e�B
	/// ���s�����ꍇ��false��Ԃ�
	///</summary>
	bool  MySQLDb::create_table(const  TableStructure& table_info)
	{
		const std::string query =  QueryGenerateUtility::make_create_table_query(table_info);
		return execute(query);
	}


	///<summary>
	/// INSERT�����s���郆�[�e�B���e�B
	/// ���s�����ꍇ��false��Ԃ�
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
	/// INSERT�����s���郆�[�e�B���e�B
	/// ���s�����ꍇ��false��Ԃ�
	///</summary>
	bool  MySQLDb::insert(const TableStructure& insert_columns, std::shared_ptr<Bindable const> data)
	{
		return insert(insert_columns.table_name, insert_columns.get_column_names(), data);
	}


	///<summary>
	/// INSERT�����s���郆�[�e�B���e�B
	/// �����f�[�^���C���T�[�g����ꍇ�͂�������g���ƌ������悢
	/// ���s�����ꍇ��false��Ԃ�
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
	/// INSERT�����s���郆�[�e�B���e�B
	/// �����f�[�^���C���T�[�g����ꍇ�͂�������g���ƌ������悢
	/// ���s�����ꍇ��false��Ԃ�
	///</summary>
	bool  MySQLDb::insert(const  TableStructure& insert_columns, const std::list<std::shared_ptr<Bindable const>>& data_list)
	{
		return insert(insert_columns.table_name, insert_columns.get_column_names(), data_list);
	}


	///<summary>
	/// UPDATE�����s���郆�[�e�B���e�B
	/// �ύX���ꂽ�J��������Ԃ�
	/// ���s�����ꍇ��-1��Ԃ�
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
	/// UPDATE�����s���郆�[�e�B���e�B
	/// �ύX���ꂽ�J��������Ԃ�
	/// ���s�����ꍇ��-1��Ԃ�
	///</summary>
	int  MySQLDb::update(const  TableStructure& update_columns, std::shared_ptr<Bindable const> data, std::string where_clause)
	{
		return update(update_columns.table_name, update_columns.get_column_names(), data, where_clause);
	}



	///<summary>
	/// �ȒP��SELECT�N�G�������s���郆�[�e�B���e�B
	/// ���̃|�C���^��p���Ă���̂�delete��Y�ꂸ�s������!
	///</summary>
	sql::ResultSet*  MySQLDb::select(const std::string& table_name, const std::vector<std::string>& columns, const std::string& where_clause)
	{
		const std::string query =  QueryGenerateUtility::make_select_query(table_name, columns, where_clause);
		return std::move(raw_query(query));
	}

	///<summary>
	/// �ȒP��SELECT�N�G�������s���郆�[�e�B���e�B
	/// ���̃|�C���^��p���Ă���̂�delete��Y�ꂸ�s������!
	///</summary>
	sql::ResultSet*  MySQLDb::select(const  TableStructure& select_columns, const std::string where_clause)
	{
		return std::move(select(select_columns.table_name, select_columns.get_column_names(), where_clause));
	}
}

