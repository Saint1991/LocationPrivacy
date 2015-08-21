#pragma once
#include "DbSettings.h"

namespace Db
{

	///<summary>
	/// DB設定の読み込み方法に関するインターフェース
	///</summary>
	class IDbSettingsLoader
	{
	public:
		virtual Db::DbSettings load_db_settings() const = 0;
	};
}


