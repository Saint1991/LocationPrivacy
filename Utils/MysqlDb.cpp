#include "stdafx.h"
#include "MysqlDb.h"


Db::MysqlDb::MysqlDb()
{
	try {
		int b = 3;
		boost::scoped_ptr<int> a(new int(3));

	} catch (sql::SQLException&e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}


Db::MysqlDb::~MysqlDb()
{
}
