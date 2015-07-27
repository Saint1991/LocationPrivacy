#ifdef UTILS_EXPORTS
#define FILE_EXPORTER_API __declspec(dllexport)
#else
#define FILE_EXPORTER_API __declspec(dllimport)
#endif

#pragma once
#include "FileExportable.h"

namespace Export {

	enum ExportType {TSV, CSV};

	///<summary>
	/// FileExportableを継承しているクラスをファイルに出力するためのヘルパークラス
	/// コンストラクタで指定したファイルに，指定した形式で出力する．
	/// export_name_mapは出力したいデータを受け取る時の
	/// 出力ファイルに記載されるタブのカラム名と実際のフィールドのマッピング
	/// 例えば{'a' => value1 'b' => value2}というハッシュマップを{'a'=>'住所', 'b' => '氏名'}というexport_name_mapで出力すると
	/// 住所, 氏名
	/// value1, value2
	/// value1, value2...
	///という出力になる．
	/// またこのときの対応のキーに記載されていないフィールドはエクスポートされない．
	/// (若干不便だけどC++にはリフレクションがないようなのでしゃーない...)
	///</summary>
	class FILE_EXPORTER_API FileExporter
	{
	private:
		std::ofstream out_file;
		std::unique_ptr<std::unordered_map<std::string, int>> key_position_map;
		const std::string DELIMITER;

	public:
		FileExporter(std::string outfile_path, std::list<std::pair<std::string, std::string>> export_name_map, ExportType type = ExportType::CSV);
		virtual ~FileExporter();

		void export_line(std::unordered_map<std::string, std::string> name_value_pairs);
		void export_line(std::shared_ptr<FileExportable const> data);	
		void export_lines(std::list<std::shared_ptr<FileExportable const>> data_list);
	};
}


