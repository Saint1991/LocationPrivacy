#include "stdafx.h"


namespace Export {
	
	class FileExportable {
	public:
		FileExportable();
		~FileExportable();
		virtual std::hash_map<std::string, std::string> get_export_data()  const = 0;
	};
}
