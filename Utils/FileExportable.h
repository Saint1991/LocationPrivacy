#pragma once
#include "stdafx.h"


namespace Export {
	
	class FileExportable {
	public:
		virtual std::hash_map<std::string, std::string> get_export_data()  const = 0;
	};
}
