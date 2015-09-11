#ifdef SIMULATORCOMPONENTS_EXPORTS
#define DB_NODECOLLECTION_FACTORY_API __declspec(dllexport)
#else
#define DB_NODECOLLECTION_FACTORY_API __declspec(dllimport)
#endif

#pragma once
#include "Node.h"
#include "Edge.h"
#include "NodeCollectionFactory.h"
#include "MySQLDb.h"
#include "IDbSettingsLoader.h"
#include "DbSettingsFileLoader.h"

namespace Map
{

	///<summary>
	/// データベースからノード，エッジの接続関係を読み出して
	/// NodeのIdentifiableCollectionを作成するクラス
	///</summary>
	class DB_NODECOLLECTION_FACTORY_API DbNodeCollectionFactory 
	{
	public:
		DbNodeCollectionFactory();
		~DbNodeCollectionFactory();
	};
}


