#include "stdafx.h"
#include "BaseSimulator.h"

namespace Simulation
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BaseSimulator::BaseSimulator(int user_id, double testset_proportion) 
		: ISimulator<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>(),
		testset_proportion(testset_proportion), user_id(user_id)
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
	void BaseSimulator::create_trajectories()
	{
		User::DbTrajectoryLoader<Graph::SemanticTrajectory<Geography::LatLng>> loader("../settings/mydbsettings.xml", "map_tokyo", "checkins", "pois");
		user_trajectories = loader.load_trajectories(user_id);
		first_trajectory_sequence = (int)(user_trajectories->size() * testset_proportion) + 1;
	}

	///<summary>
	/// �n�D�̖؂̍쐬 (������)
	///</summary>
	void BaseSimulator::build_user_preference_tree()
	{
		for (int trajectory_sequence = 0; trajectory_sequence < first_trajectory_sequence; trajectory_sequence++) {
			std::shared_ptr<Graph::SemanticTrajectory<>> trajectory = user_trajectories->at(trajectory_sequence);
		}
	}


	///<summary>
	/// �O��������
	///</summary>
	void BaseSimulator::prepare()
	{
		build_map();
		create_trajectories();
		build_user_preference_tree();
	}

	///<summary>
	/// �V�~�����[�^�̒�Ď�@���s����
	///</summary>
	void BaseSimulator::run()
	{

	}
}

