#include "stdafx.h"
#include "BaseSimulator.h"

namespace Simulation
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BaseSimulator::BaseSimulator() 
		: ISimulator<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement>()
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	BaseSimulator::~BaseSimulator()
	{
	}


	///<summary>
	/// Map�̍\��
	/// map_tokyo��DB����f�[�^��ǂݏo���āCWarshallFloyd�@�Ń��[�e�B���O�e�[�u�����쐬����
	///</summary>
	void BaseSimulator::build_map()
	{
		map = std::make_shared<Map::BasicDbMap>("../settings/mydbsettings.xml", "map_tokyo");
		map->initialize(std::move(std::make_unique<Graph::WarshallFloyd<Map::BasicMapNode, Map::BasicRoad>>()));
	}


	///<summary>
	/// ���[�U�̍쐬
	///</summary>
	void BaseSimulator::create_user(unsigned int user_id)
	{
		User::DbBasicUserLoader loader("../settings/mydbsettings.xml", "map_tokyo", "checkins", "pois");
		user = std::move(loader.load_user(user_id));
	}
}

