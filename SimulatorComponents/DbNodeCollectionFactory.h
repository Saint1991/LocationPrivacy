#ifdef SIMULATORCOMPONENTS_EXPORTS
#define DB_NODECOLLECTION_FACTORY_API __declspec(dllexport)
#else
#define DB_NODECOLLECTION_FACTORY_API __declspec(dllimport)
#endif

#pragma once
#include "BasicMapNode.h"
#include "BasicRoad.h"
#include "BasicPathData.h"
#include "NodeCollectionFactory.h"
#include "MySQLDb.h"
#include "IDbSettingsLoader.h"
#include "DbSettingsFileLoader.h"
#include "Rectangle.h"

namespace Map
{

	///<summary>
	/// �f�[�^�x�[�X����m�[�h�C�G�b�W�̐ڑ��֌W��ǂݏo����
	/// Node��IdentifiableCollection���쐬����N���X
	/// Table�̃t�H�[�}�b�g�͂��ꂼ��
	/// nodes (id, latitude, longitude)
	/// node_connections (id1, id2, distance)��
	/// node_connections�Ɋi�[����Ă���R�l�N�V������o�����ō\������
	///</summary>
	class DB_NODECOLLECTION_FACTORY_API DbNodeCollectionFactory : public Graph::NodeCollectionFactory<BasicMapNode, BasicRoad, Graph::BasicPathData>
	{
	private:
		std::unique_ptr<Db::MySQLDb> db;
		std::string db_name;
		std::string node_table;
		std::string connection_table;
	
	protected:
		void create_nodes(const Graph::Rectangle<Geography::LatLng>& boundary);
		void set_connectivities(const Graph::Rectangle<Geography::LatLng>& boundary);

	public:
		DbNodeCollectionFactory(
			const std::string& setting_file_path,
			const std::string& db_name,
			const std::string& node_table_name = "nodes",
			const std::string& connection_table_name = "node_connections"
		);
		~DbNodeCollectionFactory();
	};
}


