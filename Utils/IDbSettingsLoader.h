#ifdef UTILS_EXPORTS
#define IDB_SETTINGS_LOADER_API __declspec(dllexport)
#else
#define IDB_SETTINGS_LOADER_API __declspec(dllimport)
#endif

#pragma once
#include "DbSettings.h"
namespace Db
{

	///<summary>
	/// DB�ݒ�̓ǂݍ��ݕ��@�Ɋւ���C���^�[�t�F�[�X
	///</summary>
	class IDB_SETTINGS_LOADER_API IDbSettingsLoader
	{
	public:
		virtual const Db::DbSettings load_db_settings() const = 0;
	};
}


