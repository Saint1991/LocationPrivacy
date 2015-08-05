#pragma once
#include <mysql_connection.h>
#include <cppconn\driver.h>
#include <cppconn\resultset.h>
#include <cppconn\statement.h>
#include <cppconn\prepared_statement.h>

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


