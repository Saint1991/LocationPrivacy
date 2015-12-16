#include "stdafx.h"
#include "BaseSimulator.h"

namespace Simulation
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	BaseSimulator::BaseSimulator(unsigned int user_id, double trainingset_proportion, const std::string& db_name) 
		: ISimulator<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::PreferenceRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>>(),
		TRAININGSET_PROPORTION(trainingset_proportion), USER_ID(user_id), DB_NAME(db_name),
		user_preference_tree(std::make_shared<User::PreferenceTree>()),
		observed_preference_tree(std::make_shared<User::PreferenceTree>())
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	BaseSimulator::~BaseSimulator()
	{
	}


	///<summary>
	/// trajectory_boundary����ɓǂݏo���n�}�̈���v�Z���郆�[�e�B���e�B
	/// boundary_side_length�͖ڈ�
	///</summary>
	Graph::Rectangle<Geography::LatLng> BaseSimulator::calc_map_boundary(const Graph::Rectangle<Geography::LatLng>& boundary, double boundary_side_length)
	{
		double width = boundary.width();
		double height = boundary.height();

		double left_width = width > boundary_side_length ? 3000.0 : boundary_side_length - width ;
		double left_height = height > boundary_side_length ? 3000.0 : boundary_side_length - height;

		double left_lng = 0.000009 * (left_width / 2.0);
		double left_lat = 0.000007 * (left_height / 2.0);
		
		Graph::Rectangle<Geography::LatLng> ret(boundary.top + left_lat, boundary.left - left_lng, boundary.bottom - left_lat, boundary.right + left_lng);
		return ret;
	}

	///<summary>
	/// Map�̍\��
	/// map_tokyo��DB����f�[�^��ǂݏo���āCWarshallFloyd�@�Ń��[�e�B���O�e�[�u�����쐬����
	///</summary>
	void BaseSimulator::build_map(const Graph::Rectangle<Geography::LatLng>& boundary)
	{
		map = std::make_shared<Map::BasicDbMap>(std::make_shared<Graph::Dijkstra<Map::BasicMapNode, Map::BasicRoad>>(), "../settings/mydbsettings.xml", DB_NAME);
		map->load(boundary);
	}


	///<summary>
	/// �g���W�F�N�g�������̃��[��
	/// �o�ߎ�����2���Ԕ��ȏ�̂Ƃ���Ńg���W�F�N�g���𕪊�����
	///</summary>
	bool trajectory_division_rule(const std::string& timestamp, const std::string& previous_time)
	{
		constexpr time_t DIVISION_THRESHOLD = 7200;

		time_t current_time_t = Time::TimeUtility::convert_to_unixtimestamp(timestamp);
		time_t previous_time_t = Time::TimeUtility::convert_to_unixtimestamp(previous_time);

		time_t passed_time = current_time_t - previous_time_t;
		if (passed_time < DIVISION_THRESHOLD) return false;
		return true;
	}

	///<summary>
	/// DB���烆�[�U�̃g���W�F�N�g�����X�g���쐬
	///</summary>
	void BaseSimulator::create_trajectories()
	{
		User::DbTrajectoryLoader<Graph::SemanticTrajectory<Geography::LatLng>> loader(trajectory_division_rule, "../settings/mydbsettings.xml", DB_NAME, "checkins", "pois");
		user_trajectories = loader.load_trajectories(USER_ID, TRAJECTORY_LENGTH_THRESHOLD, MIN_SERVICE_INTERVAL);

		size_t trajectory_size = user_trajectories->size();
		std::cout << "Create " << trajectory_size << " Trajectories." << std::endl;
	}

	///<summary>
	/// �n�D�̖؂̍쐬
	///</summary>
	void BaseSimulator::build_user_preference_tree()
	{
		current_trajectory_id = TRAININGSET_PROPORTION * user_trajectories->size() + 1;
		for (unsigned int trajectory_id = 0; trajectory_id < current_trajectory_id; trajectory_id++) {
			std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> trajectory = user_trajectories->at(trajectory_id);
			int trajectory_length = trajectory->phase_count();
			Collection::Sequence<std::string> category_sequence = trajectory->get_category_sequence(0, trajectory_length - 1);
			user_preference_tree->add_sequence_counter(category_sequence);
			observed_preference_tree->add_sequence_counter(category_sequence);
		}
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

