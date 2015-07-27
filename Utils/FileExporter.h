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
	/// FileExportable���p�����Ă���N���X���t�@�C���ɏo�͂��邽�߂̃w���p�[�N���X
	/// �R���X�g���N�^�Ŏw�肵���t�@�C���ɁC�w�肵���`���ŏo�͂���D
	/// export_name_map�͏o�͂������f�[�^���󂯎�鎞��
	/// �o�̓t�@�C���ɋL�ڂ����^�u�̃J�������Ǝ��ۂ̃t�B�[���h�̃}�b�s���O
	/// �Ⴆ��{'a' => value1 'b' => value2}�Ƃ����n�b�V���}�b�v��{'a'=>'�Z��', 'b' => '����'}�Ƃ���export_name_map�ŏo�͂����
	/// �Z��, ����
	/// value1, value2
	/// value1, value2...
	///�Ƃ����o�͂ɂȂ�D
	/// �܂����̂Ƃ��̑Ή��̃L�[�ɋL�ڂ���Ă��Ȃ��t�B�[���h�̓G�N�X�|�[�g����Ȃ��D
	/// (�኱�s�ւ�����C++�ɂ̓��t���N�V�������Ȃ��悤�Ȃ̂ł���[�Ȃ�...)
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


