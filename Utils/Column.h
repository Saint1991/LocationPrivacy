#ifdef UTILS_EXPORTS
#define COLUMN_API __declspec(dllexport)
#else
#define COLUMN_API __declspec(dllimport)
#endif

#pragma once
#include "Serializable.h"

namespace Db 
{

	struct COLUMN_API Column : Serializable
	{
		std::string column_name;
		std::string data_type;
		std::unique_ptr<std::list<std::string>> options;
		std::string default_value;

		Column(std::string column_name, std::string type, std::list<std::string> options = {}, std::string default_value = "");
		Column(const Db::Column& column);
		virtual ~Column();
		const std::string to_string() const;
	};
}
