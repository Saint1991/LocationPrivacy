#include "stdafx.h"
#include "DbSettings.h"


///<summary>
/// �R���X�g���N�^
///</summary>
Db::DbSettings::DbSettings()
{
}


///<summary>
/// �R���X�g���N�^(��������)
///</summary>
Db::DbSettings::DbSettings(std::string hostname, unsigned int port, std::string username, std::string password) : hostname(hostname), port(port), username(username), password(password)
{
}


///<summary>
/// �f�X�g���N�^
///</summary>
Db::DbSettings::~DbSettings()
{
}

