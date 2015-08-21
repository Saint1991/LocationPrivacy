#ifdef UTILS_EXPORTS
#define DB_SETTINGS_FILE_LOADER_API __declspec(dllexport)
#else
#define DB_SETTINGS_FILE_LOADER_API __declspec(dllimport)
#endif

#pragma once
#include "IDbSettingsLoader.h"
#include "IOException.h"
#include "DbSettings.h"
#include "tinyxml2.h"

namespace Db
{
	class DB_SETTINGS_FILE_LOADER_API DbSettingsFileLoader : public IDbSettingsLoader
	{
	protected:
		std::string settings_file_path;
	public:
		DbSettingsFileLoader(const std::string& path = "../settings/dbsettings-sample.xml");
		virtual ~DbSettingsFileLoader();
		Db::DbSettings load_db_settings() const;
	};
}


