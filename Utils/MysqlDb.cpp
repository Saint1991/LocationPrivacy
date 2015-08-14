#include "stdafx.h"
#include "MysqlDb.h"
#include "mysql_connection.h"
#include "mysql_driver.h"
#include "cppconn\driver.h"
#include "cppconn\connection.h"

///<summary>
/// コンストラクタ
///</summary>
Db::MysqlDb::MysqlDb(std::unique_ptr<Db::IDbSettingsLoader> loader) : settings(std::make_unique<Db::DbSettings>(loader->load_db_settings()))
{
	
	sql::Driver* driver = get_driver_instance();
	std::string host = settings->hostname + ":" + std::to_string(settings->port);
	sql::Connection* connection = driver->connect(host, settings->username, settings->password);
	connection->setSchema("test");
	
}


///<summary>
/// デストラクタ
///</summary>
Db::MysqlDb::~MysqlDb()
{
}
