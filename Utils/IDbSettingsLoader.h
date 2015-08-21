#pragma once
#include "DbSettings.h"

namespace Db
{

	///<summary>
	/// DB�ݒ�̓ǂݍ��ݕ��@�Ɋւ���C���^�[�t�F�[�X
	///</summary>
	class IDbSettingsLoader
	{
	public:
		virtual Db::DbSettings load_db_settings() const = 0;
	};
}


