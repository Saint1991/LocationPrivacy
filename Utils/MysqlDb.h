#pragma once
#include "mysql_connection.h"
#include "mysql_driver.h"
#include <cppconn\driver.h>
#include <cppconn\exception.h>
#include <cppconn\resultset.h>
#include <cppconn\statement.h>

#include "boost\scoped_ptr.hpp"
namespace Db 
{
	class MysqlDb
	{
	protected:

	public:
		MysqlDb();
		~MysqlDb();
	};
}


