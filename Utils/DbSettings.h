#ifdef UTILS_EXPORTS
#define DB_SETTINGS_API __declspec(dllexport)
#else
#define DB_SETTINGS_API __declspec(dllimport)
#endif

#pragma once

namespace Db 
{

	///<summary>
	/// DB設定を格納する構造体
	///</summary>
	struct DbSettings
	{
		std::string hostname;
		unsigned int port;
		std::string username;
		std::string password;

		DB_SETTINGS_API DbSettings();
		DB_SETTINGS_API DbSettings(const std::string& hostname, const unsigned int port, const std::string& username, const std::string& password);
		DB_SETTINGS_API virtual ~DbSettings();
	};
}


