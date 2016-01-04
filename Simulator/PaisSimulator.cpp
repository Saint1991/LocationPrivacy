#include "stdafx.h"
#include "PaisSimulator.h"

namespace Simulation
{

	PaisSimulator::PaisSimulator() : BaseSimulator(USER_ID, DB_NAME)
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
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 1.0, AVERAGE_SPEED, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.9, AVERAGE_SPEED, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.8, AVERAGE_SPEED, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.7, AVERAGE_SPEED, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.6, AVERAGE_SPEED, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.5, AVERAGE_SPEED, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.4, AVERAGE_SPEED, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.3, AVERAGE_SPEED, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.2, AVERAGE_SPEED, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.1, AVERAGE_SPEED, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.0, AVERAGE_SPEED, 1.0)
		};
	}


	void PaisSimulator::run()
	{

		int failed_count = 0;

		//�S�g���W�F�N�g���f�[�^���烆�[�U�̚n�D�̖؂��쐬����
		

		for (std::list<std::shared_ptr<Requirement::PreferenceRequirement const>>::const_iterator iter = requirements.begin(); iter != requirements.end(); iter++) {
			for (int current_trajectory_id = 0; current_trajectory_id < user_trajectories->size(); current_trajectory_id++) {
			
				//�Ώۂ̃��[�U�g���W�F�N�g��
				std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> user_trajectory = user_trajectories->at(current_trajectory_id);
				std::shared_ptr<Time::TimeSlotManager const> timeslot = user_trajectory->read_timeslot();
				
				//�ǂݍ��ޒn�}�̗̈�
				Graph::Rectangle<Geography::LatLng> map_boundary = user_trajectory->get_trajectory_boundary();
				map_boundary = Map::BasicDbMap::calc_map_boundary(map_boundary);
				
				//�n�}�̓ǂݍ���
				build_map(map_boundary);
				User::UserTrajectoryConverter converter(map);

				//�g���W�F�N�g���ϊ�
				std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> converted_trajectory = converter.extract_walking_semantic_trajectory(user_trajectory, TRAJECTORY_LENGTH_THRESHOLD, AVERAGE_SPEED);
				if (converted_trajectory == nullptr) continue;

				//�Ώۂ̃g���W�F�N�g���Ń��[�U�̚n�D�̖؂��X�V
				user_preference_tree->add_sequence_counter(user_trajectory->get_category_sequence());

				//�ϑ��Țn�D�̖؂̃R�s�[
				std::shared_ptr<User::PreferenceTree> observed_preference_tree_copy = std::make_shared<User::PreferenceTree>(*observed_preference_tree);
				
				//���[�U�̍쐬
				std::shared_ptr<User::BasicUser<Geography::LatLng>> user = std::make_shared<User::BasicUser<Geography::LatLng>>(user_trajectory, user_preference_tree);
				
				//��Ď�@�̃C���X�^���X��
				std::shared_ptr<Framework::IProposedMethod<
					Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>,
					Requirement::PreferenceRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>
				>> proposed = std::make_shared<Method::MizunoMethod>(map, user, observed_preference_tree_copy, *iter, timeslot);
				
				//�e�g���W�F�N�g���ɑ΂��Ď�@��K�p������̃R�[���o�b�N
				proposed->set_execution_callback(std::bind(&PaisSimulator::each_trajectory_end_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
				
				try {
					//��Ď�@�̋N��
					proposed->run();
				}
				catch (Framework::TrajectoryNotFoundException &e) {
					std::cout << e.what() << std::endl;
					failed_count++;
				}
				

				//������
				break;
			}

			//������
			break;
		}

		double fail_rate = failed_count / user_trajectories->size();
		std::cout << "Fail Rate: " << std::to_string(fail_rate) << std::endl;
	}

	///<summary>
	/// �e�g���W�F�N�g�����s��̃R�[���o�b�N
	///</summary>
	void PaisSimulator::each_trajectory_end_callback(std::shared_ptr<Entity::EntityManager<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>, Entity::Dummy<>, User::BasicUser<>>> entities, std::shared_ptr<Requirement::PreferenceRequirement const> requirement, std::shared_ptr<Time::Timer> timer)
	{
		
		std::list<std::pair<std::string, std::string>> export_name_map = {
			{Geography::LatLng::LATITUDE, "�ܓx" },
			{Geography::LatLng::LONGITUDE, "�o�x" },
			{Graph::TrajectoryState<>::TIME, "�^�C���X�^���v"},
			{Graph::SemanticTrajectoryState<>::CATEGORY, "�J�e�S��ID"},
			{Graph::SemanticTrajectoryState<>::CATEGORY_NAME, "�J�e�S����"},
			{Graph::TrajectoryState<>::VENUE_NAME, "POI��"}
		};
		IO::FileExporter exporter(export_name_map, "C:/Users/Mizuno/Desktop/EvaluationResults/user_trajectory");
		
		//���[�U�g���W�F�N�g���̃G�N�X�|�[�g
		std::list<std::shared_ptr<IO::FileExportable const>> user_trajectory = entities->get_user()->read_trajectory()->get_export_data();
		exporter.export_lines(user_trajectory);

		//�e�_�~�[�̃G�N�X�|�[�g
		for (int dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++) {
			std::shared_ptr<Entity::Dummy<> const> dummy = entities->read_dummy_by_id(dummy_id);
			std::list<std::shared_ptr<IO::FileExportable const>> dummy_trajectory = dummy->read_trajectory()->get_export_data();
			IO::FileExporter dummy_exporter(export_name_map, "C:/Users/Mizuno/Desktop/EvaluationResults/dummy" + std::to_string(dummy_id));
			dummy_exporter.export_lines(dummy_trajectory);
		}

		//AR�n�̕]��
		int achive_count = 0;
		double achive_size = 0.0;
		int phase_count = entities->get_user()->read_trajectory()->phase_count();
		for (int phase = 0; phase < phase_count; phase++) {
			double ar = entities->calc_convex_hull_size_of_fixed_entities_of_phase(phase);
			if (ar >= requirement->required_anonymous_area) achive_count++;
			achive_size += ar / requirement->required_anonymous_area;
		}
		double ar_count = (double)achive_count / phase_count;
		double ar_size = achive_size / phase_count;
		std::cout << "AR-Count: " << std::to_string(ar_count) << std::endl;
		std::cout << "AR-Size: " << std::to_string(ar_size) << "m^2" << std::endl;
		
		//MTC�̕]��
		

		//�؂̍X�V
	}

	void PaisSimulator::evaluate()
	{
		
	}

	void PaisSimulator::export_evaluation_result()
	{

	}



}


