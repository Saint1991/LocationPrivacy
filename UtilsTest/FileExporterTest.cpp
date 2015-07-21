#include "stdafx.h"
#include "CppUnitTest.h"
#include "FileExporter.h"
#include "LatLng.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(FileExporterTest)
	{
	public:
		
		TEST_METHOD(export_line1)
		{
			std::string file_path = "C:/Users/Mizuno/Desktop/out-test";
			std::list <std::pair<std::string, std::string>> export_name_map;
			export_name_map.push_back(std::make_pair("latitude", "緯度"));
			export_name_map.push_back(std::make_pair("longitude", "経度"));
			Export::FileExporter out_file(file_path, export_name_map);

			std::shared_ptr<Geography::LatLng const> ll1 = std::make_shared<Geography::LatLng const>(39.9, 28.8);
			std::shared_ptr<Geography::LatLng const> ll2 = std::make_shared<Geography::LatLng const>(79.9, 128.8);
			out_file.export_line(ll1);
			out_file.export_line(ll2);
		}

		TEST_METHOD(export_line2)
		{
			std::string file_path = "C:/Users/Mizuno/Desktop/out-test";
			std::list <std::pair<std::string, std::string>> export_name_map;
			export_name_map.push_back(std::make_pair("latitude", "緯度"));
			export_name_map.push_back(std::make_pair("longitude", "経度"));
			Export::FileExporter out_file(file_path, export_name_map, Export::ExportType::TSV);

			std::shared_ptr<Geography::LatLng const> ll1 = std::make_shared<Geography::LatLng const>(39.9, 28.8);
			std::shared_ptr<Geography::LatLng const> ll2 = std::make_shared<Geography::LatLng const>(79.9, 128.8);
			out_file.export_line(ll1);
			out_file.export_line(ll2);
		}

		TEST_METHOD(export_lines)
		{
			std::string file_path = "C:/Users/Mizuno/Desktop/out-test-list";
			std::list <std::pair<std::string, std::string>> export_name_map;
			export_name_map.push_back(std::make_pair("latitude", "緯度"));
			export_name_map.push_back(std::make_pair("longitude", "経度"));
			Export::FileExporter out_file(file_path, export_name_map);

			std::shared_ptr<Geography::LatLng const> ll1 = std::make_shared<Geography::LatLng const>(39.9, 28.8);
			std::shared_ptr<Geography::LatLng const> ll2 = std::make_shared<Geography::LatLng const>(79.9, 128.8);
			std::list < std::shared_ptr < Export::FileExportable const >> data_list = { ll1, ll2 };
			out_file.export_lines(data_list);
		}

	};
}