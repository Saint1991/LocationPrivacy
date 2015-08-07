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
Db::DbSettings::DbSettings(std::string hostname, unsigned int port, std::string username, std::string password) : hostname(hostname), port(port), username(username), password(password)
{
}


///<summary>
/// デストラクタ
///</summary>
Db::DbSettings::~DbSettings()
{
}

