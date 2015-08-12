#include "stdafx.h"
#include "MysqlDb.h"


///<summary>
/// �R���X�g���N�^
///</summary>
Db::MysqlDb::MysqlDb(std::unique_ptr<Db::IDbSettingsLoader> loader)
{
	settings = std::make_unique<Db::DbSettings>(loader->load_db_settings());
	//sql::Driver* driver =  get_driver_instance();
	//sql::Driver *driver;

	//std::string host = settings->hostname + ":" + std::to_string(settings->port);
	//driver->connect(host, settings->username, settings->password);
}


///<summary>
/// �f�X�g���N�^
///</summary>
Db::MysqlDb::~MysqlDb()
{
}
