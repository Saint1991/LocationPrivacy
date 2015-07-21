#include "stdafx.h"
#include "FileExporter.h"


///<summary>
/// コンストラクタ
///</summary>
///<param name='outfile_path'>出力ファイルのパス(ただし拡張子は不要)</param>
///<param name='export_name_map'></param>
///<param name='type'>出力形式(TSV or CSV)</param>
Export::FileExporter::FileExporter(std::string outfile_path, std::list<std::pair<std::string, std::string>> export_name_map, ExportType type)
	: key_position_map(std::make_unique<std::hash_map<std::string, int>>()), DELIMITER(type == ExportType::TSV ? "\t" : ",")
{

	outfile_path += type == ExportType::TSV ? ".tsv" : ".csv";
	out_file = std::ofstream(outfile_path);
	
	int index = 0;
	std::string header("");
	for (auto iter = export_name_map.begin(); iter != export_name_map.end(); iter++) {
		std::string key = iter->first;
		key_position_map->insert(std::make_pair(key, index++));
		header += iter->second + DELIMITER;
	}
	header.erase(header.length() - DELIMITER.length());
	out_file << header << std::endl;
}


///<summary>
/// デストラクタ
///</summary>
Export::FileExporter::~FileExporter()
{
	out_file.close();
}


///<summary>
/// ハッシュマップに対応するデータをエクスポートする
///</summary>
///<param name='name_value_pairs'>コンストラクタで対応するキー=>値のマップ</param>
void Export::FileExporter::export_line(std::hash_map<std::string, std::string> name_value_pairs)
{
	int value_num = key_position_map->size();
	std::unique_ptr<std::vector<std::string>> values = std::make_unique<std::vector<std::string>>(value_num);
	for (auto iter = name_value_pairs.begin(); iter != name_value_pairs.end(); iter++) {
		std::string key = iter->first;
		if (key_position_map->find(key) != key_position_map->end()) {
			values->at(key_position_map->at(key)) = iter->second;
		}
	}

	std::string line("");
	for (auto iter = values->begin(); iter != values->end(); iter++) {
		line += *iter + DELIMITER;
	}

	line.erase(line.length() - DELIMITER.length());
	out_file << line << std::endl;
}

///<summary>
/// ハッシュマップに対応するデータをエクスポートする
///</summary>
///<param name='name_value_pairs'>コンストラクタで対応するキー=>値のマップ</param>
void Export::FileExporter::export_line(std::shared_ptr<Export::FileExportable const> data)
{
	export_line(data->get_export_data());
}


///<summary>
/// 指定したデータリストをまとめてエクスポートします．
///</summary>
///<param name=''data_list'>exportableなクラスのリスト</param>
void Export::FileExporter::export_lines(std::list<std::shared_ptr<Export::FileExportable const>> data_list)
{
	for (auto iter = data_list.begin(); iter != data_list.end(); iter++) {
		export_line(*iter);
	}
}
