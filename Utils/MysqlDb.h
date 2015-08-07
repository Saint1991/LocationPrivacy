#pragma once
#include "DbSettings.h"

namespace Db 
{
	class MysqlDb
	{
	protected:

	public:
		MysqlDb(Db::DbSettings settings);
		~MysqlDb();
	};
}


