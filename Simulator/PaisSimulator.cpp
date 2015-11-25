#include "stdafx.h"
#include "PaisSimulator.h"

namespace Simulation
{

	PaisSimulator::PaisSimulator() : BaseSimulator(USER_ID, TRAININGSET_PROPORTION, DB_NAME)
	{
	}


	PaisSimulator::~PaisSimulator()
	{
	}

	///<summary>
	/// �V�~�����[�V�����̃p�����[�^���X�g
	///</summary>
	void PaisSimulator::make_requirement_list()
	{
		requirements = {
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.0, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.1, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.2, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.3, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.4, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.5, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.6, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.7, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.8, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.9, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 1.0, 1.5, 0.5),
		};
	}

	void PaisSimulator::run()
	{
		for (std::list<std::shared_ptr<Requirement::PreferenceRequirement const>>::const_iterator iter = requirements.begin(); iter != requirements.end(); iter++) {
			for (; current_trajectory_id < user_trajectories->size(); current_trajectory_id++) {
			
				//�Ώۂ̃��[�U�g���W�F�N�g��
				std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> user_trajectory = user_trajectories->at(current_trajectory_id);
				std::shared_ptr<Time::TimeSlotManager const> timeslot = user_trajectory->read_timeslot();
				std::shared_ptr<User::BasicUser<Geography::LatLng>> user = std::make_shared<User::BasicUser<Geography::LatLng>>(user_trajectory, user_preference_tree);

				//�ǂݍ��ޒn�}�̗̈�
				Graph::Rectangle<Geography::LatLng> map_boundary = user_trajectory->get_trajectory_boundary();
				map_boundary = calc_map_boundary(map_boundary);

				//�n�}�̓ǂݍ���
				build_map(map_boundary);

				//��Ď�@�̃C���X�^���X��
				std::shared_ptr<Framework::IProposedMethod<
					Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>,
					Requirement::PreferenceRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>
				>> proposed = std::make_shared<Method::MizunoMethod>(map, user, *iter, timeslot);

				//��Ď�@�̋N��
				proposed->run();

				//������
				break;
			}

			//������
			break;
		}
	}

	void PaisSimulator::evaluate()
	{

	}

	void PaisSimulator::export_evaluation_result()
	{

	}
}


