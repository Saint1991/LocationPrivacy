#ifdef UTILS_EXPORTS
#define MYSQL_DB_API __declspec(dllexport)
#else
#define MYSQL_DB_API __declspec(dllimport)
#endif
#pragma once
#include "IDbSettingsLoader.h"
#include "DbSettings.h"


namespace Db 
{
	class MYSQL_DB_API MysqlDb
	{
	protected:
		std::unique_ptr<Db::DbSettings const> settings;
	
	public:
		MysqlDb(std::unique_ptr<Db::IDbSettingsLoader> loader);
		~MysqlDb();
	};
}


