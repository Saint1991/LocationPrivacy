#include "stdafx.h"
#include "MysqlDb.h"


///<summary>
/// �R���X�g���N�^
///</summary>
Db::MysqlDb::MysqlDb(std::unique_ptr<Db::IDbSettingsLoader> loader) : settings(std::make_unique<Db::DbSettings>(loader->load_db_settings()))
{
	sql::Driver* driver = get_driver_instance();
	std::string host = "tcp://" + settings->hostname + ":" + std::to_string(settings->port);
	connection = driver->connect(host, settings->username, settings->password);
}


///<summary>
/// �R���X�g���N�^
///</summary>
Db::MysqlDb::MysqlDb(Db::DbSettings db_config) : settings(std::make_unique<Db::DbSettings>(db_config))
{
	sql::Driver* driver = get_driver_instance();
	std::string host = "tcp://" + settings->hostname + ":" + std::to_string(settings->port);
	connection = driver->connect(host, settings->username, settings->password);
}

///<summary>
/// �f�X�g���N�^
///</summary>
Db::MysqlDb::~MysqlDb()
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
RESULT_TYPE* Db::MysqlDb::execute_if_connection_is_alive(const std::function<RESULT_TYPE(void)>& execute_function)
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
/// �g�p����DB�̑I���C�؂�ւ����s��
/// ��������true�����s����false��Ԃ�
///</summary>
bool Db::MysqlDb::use(const char* dbname)
{
	bool* result = execute_if_connection_is_alive<bool>([&] { 
		connection->setSchema(dbname);
		return true;
	});

	if (result != nullptr) return *result;
	return false;
}


///<summary>
/// CREATE TABLE���̌��ʂ�Ԃ��Ȃ��N�G�������s���܂��D
/// ��������true�����s����false��Ԃ�
///</summary>
bool Db::MysqlDb::execute(const char* query)
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
/// ������œn���ꂽ�N�G�������s���C���̌��ʂ�Ԃ�
/// �R�l�N�V�����������ȏꍇ�Ȃǂ́Cnullptr��Ԃ��܂�
/// ���̃|�C���^���g���Ă���̂ŕK��delete���邱��
///</summary>
sql::ResultSet* Db::MysqlDb::row_query(const char* query)
{
	return *(execute_if_connection_is_alive<sql::ResultSet*>([&] {
		sql::Statement* statement = connection->createStatement();
		sql::ResultSet* result = statement->executeQuery(query);
		delete statement;
		return result;
	}));
}


///<summary>
/// �f�[�^�x�[�X�ꗗ���擾����
///</summary>
const std::list<std::string> Db::MysqlDb::get_databases()
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
/// ���ݐڑ����Ă���DB���̃e�[�u���ꗗ���擾����
///</summary>
const std::list<std::string> Db::MysqlDb::get_tables()
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