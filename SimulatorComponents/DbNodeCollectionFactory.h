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

namespace Map
{

	///<summary>
	/// データベースからノード，エッジの接続関係を読み出して
	/// NodeのIdentifiableCollectionを作成するクラス
	/// Tableのフォーマットはそれぞれ
	/// nodes (id, latitude, longitude)
	/// node_connections (id1, id2, distance)で
	/// node_connectionsに格納されているコネクションを双方向で構成する
	///</summary>
	class DB_NODECOLLECTION_FACTORY_API DbNodeCollectionFactory : public Graph::NodeCollectionFactory<BasicMapNode, BasicRoad, Graph::BasicPathData>
	{
	private:
		std::unique_ptr<Db::MySQLDb> db;
		std::string db_name;
		std::string node_table;
		std::string connection_table;
	
	protected:
		void create_nodes();
		void set_connectivities();

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


