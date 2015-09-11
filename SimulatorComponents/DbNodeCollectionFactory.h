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
	/// �f�[�^�x�[�X����m�[�h�C�G�b�W�̐ڑ��֌W��ǂݏo����
	/// Node��IdentifiableCollection���쐬����N���X
	///</summary>
	class DB_NODECOLLECTION_FACTORY_API DbNodeCollectionFactory 
	{
	public:
		DbNodeCollectionFactory();
		~DbNodeCollectionFactory();
	};
}


