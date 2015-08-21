#include "stdafx.h"
#include "DbSettings.h"


///<summary>
/// コンストラクタ
///</summary>
Db::DbSettings::DbSettings()
{
}


///<summary>
/// コンストラクタ(引数あり)
///</summary>
Db::DbSettings::DbSettings(const std::string& hostname, const unsigned int port, const std::string& username, const std::string& password)
	: hostname(hostname), port(port), username(username), password(password)
{
}


///<summary>
/// デストラクタ
///</summary>
Db::DbSettings::~DbSettings()
{
}

