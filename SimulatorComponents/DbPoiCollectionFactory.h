#ifdef SIMULATORCOMPONENTS_EXPORTS
#define DB_POI_COLLECTION_FACTORY_API __declspec(dllexport)
#else
#define DB_POI_COLLECTION_FACTORY_API __declspec(dllimport)
#endif

#pragma once
#include "BasicPoi.h"
#include "BasicRoad.h"
#include "BasicPathData.h"
#include "NodeCollectionFactory.h"
#include "MySQLDb.h"
#include "IDbSettingsLoader.h"
#include "DbSettingsFileLoader.h"

namespace Map
{

	///<summary>
	/// �f�[�^�x�[�X����POI�C�����_�Ƃ̐ڑ��֌W��ǂݏo����
	/// BasicPoi��IdentifiableCollection���쐬����N���X
	/// Table�̃t�H�[�}�b�g�͂��ꂼ��
	/// pois (id, latitude, longitude, category_id, category_name, venue_name)
	/// poi_connections (id, to1, to2, distance1, distance2)��z��
	///</summary>
	class DB_POI_COLLECTION_FACTORY_API DbPoiCollectionFactory : public Graph::NodeCollectionFactory<BasicPoi, BasicRoad, Graph::BasicPathData>
	{
	private:
		std::unique_ptr<Db::MySQLDb> db;
		std::string db_name;
		std::string poi_table;
		std::string connection_table;

	protected:
		void create_nodes();
		void set_connectivities();

	public:
		DbPoiCollectionFactory(
			const std::string& setting_file_path,
			const std::string& db_name,
			const std::string& poi_table_name = "pois",
			const std::string& connection_table_name = "poi_connections"
		);
		~DbPoiCollectionFactory();
	};
}


