#include "stdafx.h"
#include "DbSettingsFileLoader.h"


namespace Db 
{
	///<summary>
	/// コンストラクタ
	///</summary>
	 DbSettingsFileLoader::DbSettingsFileLoader(const std::string& path) : settings_file_path(path)
	{
	}

	 DbSettingsFileLoader::~DbSettingsFileLoader()
	{
	}


	///<summary>
	/// 指定したパスのファイルからDB設定を読み込む
	/// ファイルが見つからない場合IOExceptionをスローする
	/// 空文字列が入っているとなぜかクラッシュするので注意!!
	///(XMLElementは明示的にデストラクタを呼ぶ必要がない仕様になっているらしい)
	///</summary>
	 DbSettings  DbSettingsFileLoader::load_db_settings() const
	{

		tinyxml2::XMLDocument document;
		tinyxml2::XMLError error = document.LoadFile(settings_file_path.c_str());
		if (error != 0) throw IO::IOException("Can't Open DB Setting File");

		//host名取得
		tinyxml2::XMLElement* settings = document.FirstChildElement("settings");
		if (settings == nullptr) {
			throw std::invalid_argument("Invalid Format of DB Setting File " + settings_file_path);
		}

		tinyxml2::XMLElement* host_node = settings->FirstChildElement("host");
		tinyxml2::XMLElement* port_node = settings->FirstChildElement("port");
		tinyxml2::XMLElement* user_node = settings->FirstChildElement("user");
		tinyxml2::XMLElement* password_node = settings->FirstChildElement("password");
		if (host_node == nullptr || port_node == nullptr || user_node == nullptr || password_node == nullptr) {
			throw std::invalid_argument("Invalid Format of DB Setting File " + settings_file_path);
		}

		//host
		std::string host = host_node->GetText();

		//port
		std::istringstream temp(port_node->GetText());
		unsigned int port = 0;
		temp >> port;

		//username
		std::string user = user_node->GetText();

		//password
		std::string password = password_node->GetText();

		return  DbSettings(host, port, user, password);
	}
}
