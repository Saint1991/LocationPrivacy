#include "stdafx.h"
#include "CppUnitTest.h"
#include "FileExporter.h"
#include "FileExportable.h"
#include "LatLng.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{

	constexpr char* USERNAME = "Mizuno";
	
	///このテストはFileExporterの使い方を示すためのもので出力の内容まではチェックしていません
	///USERNAMEを自身のものに変更するとデスクトップにサンプルの出力結果が出力されます．
	TEST_CLASS(FileExporterTest)
	{
	public:

		TEST_METHOD(FileExporter_export_line1)
		{
			std::stringstream path;
			path << "C:/Users/" << USERNAME << "/Desktop/out-test";
			
			std::list<std::pair<std::string, std::string>> export_name_map = {
				{Geography::LatLng::LATITUDE, "緯度"},
				{Geography::LatLng::LONGITUDE, "経度"}
			};
			IO::FileExporter exporter(export_name_map, path.str(), IO::ExportType::CSV);
			exporter.export_line(std::make_shared<Geography::LatLng const>(35.5, 135.9));
		}

		TEST_METHOD(FileExporter_export_line2) 
		{
			std::stringstream path;
			path << "C:/Users/" << USERNAME << "/Desktop/out-test2";

			std::list<std::pair<std::string, std::string>> export_name_map = {
				{ Geography::LatLng::LATITUDE, "緯度" },
				{ Geography::LatLng::LONGITUDE, "経度" }
			};
			IO::FileExporter exporter(export_name_map, path.str(), IO::ExportType::CSV);
			exporter.export_line({
				{Geography::LatLng::LATITUDE, "35.5"},
				{Geography::LatLng::LONGITUDE, "135.9"}
			});
		}

		TEST_METHOD(FileExporter_export_lines)
		{
			std::stringstream path;
			path << "C:/Users/" << USERNAME << "/Desktop/out-test3";

			std::list<std::pair<std::string, std::string>> export_name_map = {
				{ Geography::LatLng::LATITUDE, "緯度" },
				{ Geography::LatLng::LONGITUDE, "経度" }
			};
			IO::FileExporter exporter(export_name_map, path.str(), IO::ExportType::CSV);

			std::list<std::shared_ptr<IO::FileExportable const>> export_datas = {
				std::make_shared<Geography::LatLng const>(35.5, 135.5),
				std::make_shared<Geography::LatLng const>(35.6, 135.7)
			};
			exporter.export_lines(export_datas);
		}
	};
}
