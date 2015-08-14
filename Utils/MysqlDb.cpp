#include "stdafx.h"
#include "MysqlDb.h"
#include "mysql_connection.h"
#include "mysql_driver.h"
#include "cppconn\driver.h"
#include "cppconn\connection.h"

///<summary>
/// �R���X�g���N�^
///</summary>
Db::MysqlDb::MysqlDb(std::unique_ptr<Db::IDbSettingsLoader> loader) : settings(std::make_unique<Db::DbSettings>(loader->load_db_settings()))
{
	
	sql::Driver* driver = get_driver_instance();
	std::string host = settings->hostname + ":" + std::to_string(settings->port);
	sql::Connection* connection = driver->connect(host, settings->username, settings->password);
	connection->setSchema("test");
	
}


///<summary>
/// �f�X�g���N�^
///</summary>
Db::MysqlDb::~MysqlDb()
{
}
