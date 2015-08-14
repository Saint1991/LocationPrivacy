// LocationPrivacy.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "MysqlDb.h"
#include "DbSettingsFileLoader.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Db::MysqlDb db(std::make_unique<Db::DbSettingsFileLoader>(
		Db::DbSettingsFileLoader("C:/Users/Mizuno/Documents/Visual Studio 2015/Projects/LocationPrivacy/settings/dbsettings-sample.xml"))
	);
	
	return 0;
}

