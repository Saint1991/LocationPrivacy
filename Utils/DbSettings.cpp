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
Db::DbSettings::DbSettings(const std::string& hostname, const unsigned int port, const std::string& username, const std::string& password)
	: hostname(hostname), port(port), username(username), password(password)
{
}


///<summary>
/// �f�X�g���N�^
///</summary>
Db::DbSettings::~DbSettings()
{
}

