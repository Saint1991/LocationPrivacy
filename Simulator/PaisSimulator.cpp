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
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 1.0, 5.0, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.9, 5.0, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.8, 5.0, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.7, 5.0, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.6, 5.0, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.5, 5.0, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.4, 5.0, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.3, 5.0, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.2, 5.0, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.1, 5.0, 1.0),
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1000.0, 2), 16, 0.0, 5.0, 1.0)
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
				
				//各トラジェクトリに対して手法を適用した後のコールバック
				proposed->set_execution_callback(std::bind(&PaisSimulator::each_trajectory_end_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
				
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

	///<summary>
	/// 各トラジェクトリ実行後のコールバック
	///</summary>
	void PaisSimulator::each_trajectory_end_callback(std::shared_ptr<Entity::EntityManager<Entity::Dummy<>, User::BasicUser<>, Geography::LatLng>> entities, std::shared_ptr<Requirement::PreferenceRequirement const> requirement, std::shared_ptr<Time::Timer> timer)
	{
		
		std::list<std::pair<std::string, std::string>> export_name_map = {
			{Geography::LatLng::LATITUDE, "緯度" },
			{Geography::LatLng::LONGITUDE, "経度" },
			{Graph::TrajectoryState<>::TIME, "タイムスタンプ"},
			{Graph::SemanticTrajectoryState<>::CATEGORY, "カテゴリID"},
			{Graph::SemanticTrajectoryState<>::CATEGORY_NAME, "カテゴリ名"},
			{Graph::TrajectoryState<>::VENUE_NAME, "POI名"}
		};
		IO::FileExporter exporter(export_name_map, "C:/Users/Mizuno/Desktop/EvaluationResults/user_trajectory");
		
		//ユーザトラジェクトリのエクスポート
		std::list<std::shared_ptr<IO::FileExportable const>> user_trajectory = entities->get_user()->read_trajectory()->get_export_data();
		exporter.export_lines(user_trajectory);

		//各ダミーのエクスポート
		for (int dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++) {
			std::shared_ptr<Entity::Dummy<> const> dummy = entities->read_dummy_by_id(dummy_id);
			std::list<std::shared_ptr<IO::FileExportable const>> dummy_trajectory = dummy->read_trajectory()->get_export_data();
			IO::FileExporter dummy_exporter(export_name_map, "C:/Users/Mizuno/Desktop/EvaluationResults/dummy" + std::to_string(dummy_id));
			dummy_exporter.export_lines(dummy_trajectory);
		}

		//AR系の評価
		int achive_count = 0;
		double achive_size = 0.0;
		int phase_count = this->time_manager->phase_count();
		for (int phase = 0; phase < phase_count; phase++) {
			double ar = entities->calc_convex_hull_size_of_fixed_entities_of_phase(phase);
			if (ar >= requirement->required_anonymous_area) achive_count++;
			achive_size += ar / requirement->required_anonymous_area;
		}
		double ar_count = (double)achive_count / phase_count;
		double ar_size = achive_size / phase_count;

		//MTCの評価

		//木の更新
	}

	void PaisSimulator::evaluate()
	{
		
	}

	void PaisSimulator::export_evaluation_result()
	{

	}



}


