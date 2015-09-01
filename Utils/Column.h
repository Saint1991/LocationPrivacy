#ifdef UTILS_EXPORTS
#define COLUMN_API __declspec(dllexport)
#else
#define COLUMN_API __declspec(dllimport)
#endif

#pragma once
#include "Serializable.h"
#include "cppconn\datatype.h"

namespace Db 
{

	struct COLUMN_API Column : Serializable
	{
		std::string column_name;
		std::string data_type;
		std::unique_ptr<std::list<std::string>> options;
		std::string default_value;

		Column(const std::string& column_name, const std::string& type, std::list<std::string> options = {}, const std::string& default_value = "");
		Column(const Db::Column& column);
		virtual ~Column();
		const std::string to_string() const;

		friend bool operator==(const Column& column1, const Column& column2);
		friend bool operator!=(const Column& column1, const Column& column2);
	};
}
