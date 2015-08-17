#ifdef UTILS_EXPORTS
#define DB_SETTINGS_API __declspec(dllexport)
#else
#define DB_SETTINGS_API __declspec(dllimport)
#endif

#pragma once

namespace Db 
{

	///<summary>
	/// DBê›íËÇäiî[Ç∑ÇÈç\ë¢ëÃ
	///</summary>
	struct DbSettings
	{
		std::string hostname;
		unsigned int port;
		std::string username;
		std::string password;

		DB_SETTINGS_API DbSettings();
		DB_SETTINGS_API DbSettings(std::string hostname, unsigned int port, std::string username, std::string password);
		DB_SETTINGS_API virtual ~DbSettings();
	};
}


