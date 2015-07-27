#pragma once
#include "stdafx.h"


namespace Export {
	
	class FileExportable {
	public:
		virtual std::unordered_map<std::string, std::string> get_export_data()  const = 0;
	};
}
