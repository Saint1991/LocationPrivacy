#include "stdafx.h"
#include "BaseSimulator.h"

namespace Simulation
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BaseSimulator::BaseSimulator() 
		: ISimulator<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>()
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
	/// DB���烆�[�U�̃g���W�F�N�g�����X�g���쐬
	///</summary>
	void BaseSimulator::create_trajectories(unsigned int user_id)
	{
		User::DbTrajectoryLoader<Graph::SemanticTrajectory<Geography::LatLng>> loader("../settings/mydbsettings.xml", "map_tokyo", "checkins", "pois");
		user_trajectories = loader.load_trajectories(user_id);
	}

	///<summary>
	/// ���[�U�̍쐬
	///</summary>
	void BaseSimulator::create_user()
	{
		
	}
}

