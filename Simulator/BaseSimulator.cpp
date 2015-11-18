#include "stdafx.h"
#include "BaseSimulator.h"

namespace Simulation
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BaseSimulator::BaseSimulator(unsigned int user_id, double trainingset_proportion) 
		: ISimulator<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>(),
		TRAININGSET_PROPORTION(trainingset_proportion), USER_ID(user_id)
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
	void BaseSimulator::build_map(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		map = std::make_shared<Map::BasicDbMap>(std::make_shared<Graph::Dijkstra<Map::BasicMapNode, Map::BasicRoad>>(), "../settings/mydbsettings.xml", "map_tokyo");
		map->load(boundary);
	}


	///<summary>
	/// DB���烆�[�U�̃g���W�F�N�g�����X�g���쐬
	///</summary>
	void BaseSimulator::create_trajectories()
	{
		User::DbTrajectoryLoader<Graph::SemanticTrajectory<Geography::LatLng>> loader("../settings/mydbsettings.xml", "map_tokyo", "checkins", "pois");
		user_trajectories = loader.load_trajectories(USER_ID);
	}

	///<summary>
	/// �n�D�̖؂̍쐬 (������)
	///</summary>
	void BaseSimulator::build_user_preference_tree()
	{
	}


	///<summary>
	/// �O��������
	///</summary>
	void BaseSimulator::prepare()
	{
		make_requirement_list();
		create_trajectories();
		build_user_preference_tree();
	}
}

