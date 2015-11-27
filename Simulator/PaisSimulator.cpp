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
	/// シミュレーションのパラメータリスト
	///</summary>
	void PaisSimulator::make_requirement_list()
	{
		requirements = {
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 1.0, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.9, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.8, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.7, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.6, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.5, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.4, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.3, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.2, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.1, 1.5, 0.5),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.0, 1.5, 0.5)
		};
	}

	void PaisSimulator::run()
	{

		int failed_count = 0;

		for (std::list<std::shared_ptr<Requirement::PreferenceRequirement const>>::const_iterator iter = requirements.begin(); iter != requirements.end(); iter++) {
			for (; current_trajectory_id < user_trajectories->size(); current_trajectory_id++) {
			
				//対象のユーザトラジェクトリ
				std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> user_trajectory = user_trajectories->at(current_trajectory_id);
				std::shared_ptr<Time::TimeSlotManager const> timeslot = user_trajectory->read_timeslot();
				
				//対象のトラジェクトリでユーザの嗜好の木を更新
				user_preference_tree->add_sequence_counter(user_trajectory->get_category_sequence());

				//可観測な嗜好の木のコピー
				std::shared_ptr<User::PreferenceTree> observed_preference_tree_copy = std::make_shared<User::PreferenceTree>(*observed_preference_tree);
				
				//ユーザの作成
				std::shared_ptr<User::BasicUser<Geography::LatLng>> user = std::make_shared<User::BasicUser<Geography::LatLng>>(user_trajectory, user_preference_tree);
				
				//読み込む地図の領域
				Graph::Rectangle<Geography::LatLng> map_boundary = user_trajectory->get_trajectory_boundary();
				map_boundary = calc_map_boundary(map_boundary);

				//地図の読み込み
				build_map(map_boundary);

				//提案手法のインスタンス化
				std::shared_ptr<Framework::IProposedMethod<
					Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>,
					Requirement::PreferenceRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>
				>> proposed = std::make_shared<Method::MizunoMethod>(map, user, observed_preference_tree_copy, *iter, timeslot);
				
				try {
					//提案手法の起動
					proposed->run();
				}
				catch (Framework::TrajectoryNotFoundException &e) {
					std::cout << e.what() << std::endl;
					failed_count++;
				}
				

				//今だけ
				break;
			}

			//今だけ
			break;
		}

		double fail_rate = failed_count / (user_trajectories->size() * (1 - TRAININGSET_PROPORTION));
		std::cout << "Fail Rate: " << std::to_string(fail_rate) << std::endl;
	}

	void PaisSimulator::evaluate()
	{

	}

	void PaisSimulator::export_evaluation_result()
	{

	}

}


