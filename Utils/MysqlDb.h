#pragma once
#include "mysql_connection.h"
#include <cppconn\driver.h>
#include <cppconn\exception.h>
#include <cppconn\resultset.h>
#include <cppconn\statement.h>

namespace Db
{
	class MysqlDb
	{
	private:
		std::string dbname;

	public:
		MysqlDb();
		virtual ~MysqlDb();
	};
}


