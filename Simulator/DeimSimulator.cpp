#include "stdafx.h"
#include "DeimSimulator.h"

namespace Simulation
{

	DeimSimulator::DeimSimulator() : BaseSimulator(USER_ID, DB_NAME), current_trajectory_id(0)
	{
	}


	DeimSimulator::~DeimSimulator()
	{
	}

	///<summary>
	/// シミュレーションのパラメータリスト
	///</summary>
	void DeimSimulator::make_requirement_list()
	{
		requirements = {
			std::make_shared<Requirement::PreferenceRequirement>(std::pow(1500.0, 2), 9, 1.0, AVERAGE_SPEED, 1.0)
		};
	}


	void DeimSimulator::run()
	{
		simulation_start_time = Time::TimeUtility::current_timestamp();

		for (std::list<std::shared_ptr<Requirement::PreferenceRequirement const>>::const_iterator iter = requirements.begin(); iter != requirements.end(); iter++) {
			for (current_trajectory_id = 0; current_trajectory_id < user_trajectories->size(); current_trajectory_id++) {
			
				//対象のユーザトラジェクトリ
				std::shared_ptr<Graph::SemanticTrajectory<Geography::LatLng>> user_trajectory = user_trajectories->at(current_trajectory_id);
				std::shared_ptr<Time::TimeSlotManager const> timeslot = user_trajectory->read_timeslot();

				//可観測な嗜好の木のコピー
				std::shared_ptr<User::PreferenceTree> observed_preference_tree_copy = std::make_shared<User::PreferenceTree>(*observed_preference_tree);

				//ユーザの作成
				std::shared_ptr<User::BasicUser<Geography::LatLng>> user = std::make_shared<User::BasicUser<Geography::LatLng>>(user_trajectory, user_preference_tree);

				//提案手法のインスタンス化
				std::shared_ptr<Framework::IProposedMethod<
					Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>,
					Requirement::PreferenceRequirement, Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>
				>> proposed = std::make_shared<Method::MizunoMethodModWithoutReachability>(map, user, observed_preference_tree_copy, *iter, timeslot);
				proposed->set_execution_callback(std::bind(&DeimSimulator::each_trajectory_end_callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
				proposed->run();
			}

			//1要求当たりの結果をエクスポートする
			export_evaluation_result(**iter);
			observed_preference_tree = std::make_shared<User::PreferenceTree>();
		}
	}

	///<summary>
	/// 各トラジェクトリ実行後のコールバック
	///</summary>
	void DeimSimulator::each_trajectory_end_callback(std::shared_ptr<Entity::EntityManager<Geography::LatLng, Graph::SemanticTrajectory<Geography::LatLng>, Entity::Dummy<>, User::BasicUser<>>> entities, std::shared_ptr<Requirement::PreferenceRequirement const> requirement, std::shared_ptr<Time::Timer> timer)
	{
		std::stringstream export_path;
		export_path << "C:/Users/Mizuno/Desktop/EvaluationResults/" << simulation_start_time;
		IO::FileExporter::mkdir(export_path.str().c_str());

		export_path << "/" << requirement->dummy_num << "-" << (int)requirement->required_anonymous_area << "-" << (int)(requirement->required_preference_conservation * 100);
		IO::FileExporter::mkdir(export_path.str().c_str());

		export_path << "/" << current_trajectory_id;
		IO::FileExporter::mkdir(export_path.str().c_str());

		std::list<std::pair<std::string, std::string>> export_name_map = {
			{Geography::LatLng::LATITUDE, "緯度" },
			{Geography::LatLng::LONGITUDE, "経度" },
			{Graph::TrajectoryState<>::TIME, "タイムスタンプ"},
			{Graph::SemanticTrajectoryState<>::CATEGORY, "カテゴリID"},
			{Graph::SemanticTrajectoryState<>::CATEGORY_NAME, "カテゴリ名"},
			{Graph::TrajectoryState<>::VENUE_NAME, "POI名"}
		};
		IO::FileExporter exporter(export_name_map, export_path.str() + "/user_trajectory");
		
		//ユーザトラジェクトリのエクスポート
		std::list<std::shared_ptr<IO::FileExportable const>> user_trajectory = entities->get_user()->read_trajectory()->get_export_data();
		exporter.export_lines(user_trajectory);

		//各ダミーのエクスポート
		for (int dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++) {
			std::shared_ptr<Entity::Dummy<> const> dummy = entities->read_dummy_by_id(dummy_id);
			std::list<std::shared_ptr<IO::FileExportable const>> dummy_trajectory = dummy->read_trajectory()->get_export_data();
			IO::FileExporter dummy_exporter(export_name_map, export_path.str() +  + "/dummy" + std::to_string(dummy_id));
			dummy_exporter.export_lines(dummy_trajectory);
		}

		std::shared_ptr<Observer::SemanticObserver<Entity::Dummy<Geography::LatLng>, User::BasicUser<Geography::LatLng>>> observer
			= std::make_shared<Observer::SemanticObserver<Entity::Dummy<Geography::LatLng>, User::BasicUser<Geography::LatLng>>>(map, entities, user_preference_tree, (AVERAGE_SPEED + 1.0) * 1000 / 3600.0);
		
		//AR系の評価
		double ar_count = observer->calc_ar_count(requirement->required_anonymous_area);
		ar_count_vector_proposed.push_back(ar_count);
		std::cout << "AR-Count: " << std::to_string(ar_count) << std::endl;

		
		double ar_size = observer->calc_ar_size(requirement->required_anonymous_area);
		ar_size_vector_proposed.push_back(ar_size);
		std::cout << "AR-Size: " << std::to_string(ar_size) << "m^2" << std::endl;

		//MTCの評価
		double mtc1 = observer->calc_mtc_without_semantics();
		mtc1_vector_proposed.push_back(mtc1);
		double confusion_achieve_ratio_without_semantics = observer->get_confusion_achive_ratio_without_semantics();
		confusion_achive_ratio_without_semantics_vector.push_back(confusion_achieve_ratio_without_semantics);

		//std::cout << "MTC1: " << std::to_string(mtc1) << "sec" << std::endl;
		
		double mtc2 = observer->calc_mtc_with_semantics();
		mtc2_vector_proposed.push_back(mtc2);
		double confusion_achieve_ratio_with_semantics = observer->get_confusion_achieve_ratio_with_semantics();
		confusion_achive_ratio_with_semantics_vector.push_back(confusion_achieve_ratio_with_semantics);
		//std::cout << "MTC2: " << std::to_string(mtc2) << "sec" << std::endl;

		std::vector<int> cross_count = observer->get_cross_count_of_each_entity();
		cross_count_info.push_back(cross_count);

		//可観測な木の更新
		observer->for_each_category_sequence_of_possible_trajectory([&](const Collection::Sequence<User::category_id>& sequence, double expected_frequency) {
			observed_preference_tree->add_sequence_counter(sequence, expected_frequency);
		});

		double similarity = User::similarity(*user_preference_tree, *observed_preference_tree);
		std::cout << "Similarity: " << std::to_string(similarity) << std::endl;
		similarity_vector_proposed.push_back(similarity);

	}


	void DeimSimulator::export_evaluation_result(const Requirement::PreferenceRequirement& requirement)
	{
		//嗜好の保存度のエクスポート
		export_similarities(requirement);

		//AR-Countのエクスポート
		export_ar_counts(requirement);

		//AR-Sizeのエクスポート
		export_ar_sizes(requirement);

		//MTC1, MTC2のエクスポート
		export_mtcs(requirement);

		//交差情報のエクスポート
		export_cross_count_info(requirement);

		//嗜好のヒストグラムのエクスポート
		export_preference_histogram(requirement);

		//Confusion達成率のエクスポート
		export_confusion_ratio(requirement);

		similarity_vector_proposed = std::vector<double>();
		mtc1_vector_proposed = std::vector<double>();
		mtc2_vector_proposed = std::vector<double>();
		ar_count_vector_proposed = std::vector<double>();
		ar_size_vector_proposed = std::vector<double>();
		cross_count_info = std::vector<std::vector<int>>();
		confusion_achive_ratio_without_semantics_vector = std::vector<double>();
		confusion_achive_ratio_with_semantics_vector = std::vector<double>();
	}

	void DeimSimulator::export_similarities(const Requirement::PreferenceRequirement& requirement)
	{
		std::stringstream export_path;
		export_path << "C:/Users/Mizuno/Desktop/EvaluationResults/" << simulation_start_time;
		export_path << "/" << requirement.dummy_num << "-" << (int)requirement.required_anonymous_area << "-" << (int)(requirement.required_preference_conservation * 100);
		export_path << "/conservation.csv";
		
		std::ofstream out_file(export_path.str());
		for (std::vector<double>::const_iterator iter = similarity_vector_proposed.begin(); iter != similarity_vector_proposed.end(); iter++) {
			out_file << *iter << std::endl;
		}
		out_file.close();
	}

	void DeimSimulator::export_ar_counts(const Requirement::PreferenceRequirement& requirement)
	{
		std::stringstream export_path;
		export_path << "C:/Users/Mizuno/Desktop/EvaluationResults/" << simulation_start_time;
		export_path << "/" << requirement.dummy_num << "-" << (int)requirement.required_anonymous_area << "-" << (int)(requirement.required_preference_conservation * 100);
		export_path << "/ar-count.csv";

		std::ofstream out_file(export_path.str());
		for (std::vector<double>::const_iterator iter = ar_count_vector_proposed.begin(); iter != ar_count_vector_proposed.end(); iter++) {
			out_file << *iter << std::endl;
		}

		out_file.close();
	}

	void DeimSimulator::export_cross_count_info(const Requirement::PreferenceRequirement& requirement)
	{
		std::stringstream export_path;
		export_path << "C:/Users/Mizuno/Desktop/EvaluationResults/" << simulation_start_time;
		export_path << "/" << requirement.dummy_num << "-" << (int)requirement.required_anonymous_area << "-" << (int)(requirement.required_preference_conservation * 100);
		export_path << "/cross-count-info.csv";

		std::ofstream out_file(export_path.str());
		for (std::vector<std::vector<int>>::const_iterator iter = cross_count_info.begin(); iter != cross_count_info.end(); iter++) {
			for (Entity::entity_id id = 0; id < iter->size(); id++) {
				int count = iter->at(id);
				out_file << count << ",";
				std::cout << count << ",";
			}
			out_file << std::endl;
		}
		out_file.close();
	}

	void DeimSimulator::export_ar_sizes(const Requirement::PreferenceRequirement& requirement)
	{
		std::stringstream export_path;
		export_path << "C:/Users/Mizuno/Desktop/EvaluationResults/" << simulation_start_time;
		export_path << "/" << requirement.dummy_num << "-" << (int)requirement.required_anonymous_area << "-" << (int)(requirement.required_preference_conservation * 100);
		export_path << "/ar-size.csv";

		std::ofstream out_file(export_path.str());
		for (std::vector<double>::const_iterator iter =ar_size_vector_proposed.begin(); iter != ar_size_vector_proposed.end(); iter++) {
			out_file << *iter << std::endl;
		}

		out_file.close();
	}

	void DeimSimulator::export_mtcs(const Requirement::PreferenceRequirement& requirement)
	{
		std::stringstream export_path;
		export_path << "C:/Users/Mizuno/Desktop/EvaluationResults/" << simulation_start_time;
		export_path << "/" << requirement.dummy_num << "-" << (int)requirement.required_anonymous_area << "-" << (int)(requirement.required_preference_conservation * 100);
		export_path << "/mtc1.csv";

		std::ofstream out_file(export_path.str());
		for (std::vector<double>::const_iterator iter = mtc1_vector_proposed.begin(); iter != mtc1_vector_proposed.end(); iter++) {
			out_file << *iter << std::endl;
		}
		out_file.close();


		std::stringstream export_path2;
		export_path2 << "C:/Users/Mizuno/Desktop/EvaluationResults/" << simulation_start_time;
		export_path2 << "/" << requirement.dummy_num << "-" << (int)requirement.required_anonymous_area << "-" << (int)(requirement.required_preference_conservation * 100);
		export_path2 << "/mtc2.csv";

		out_file = std::ofstream(export_path2.str());
		for (std::vector<double>::const_iterator iter = mtc2_vector_proposed.begin(); iter != mtc2_vector_proposed.end(); iter++) {
			out_file << *iter << std::endl;
		}
		out_file.close();
	}

	void DeimSimulator::export_confusion_ratio(const Requirement::PreferenceRequirement& requirement)
	{
		std::stringstream export_path;
		export_path << "C:/Users/Mizuno/Desktop/EvaluationResults/" << simulation_start_time;
		export_path << "/" << requirement.dummy_num << "-" << (int)requirement.required_anonymous_area << "-" << (int)(requirement.required_preference_conservation * 100);
		export_path << "/confusion_ratio1.csv";
		
		std::ofstream out_file(export_path.str());
		for (std::vector<double>::const_iterator iter = confusion_achive_ratio_without_semantics_vector.begin(); iter != confusion_achive_ratio_without_semantics_vector.end(); iter++) {
			out_file << *iter << std::endl;
		}
		out_file.close();

		std::stringstream export_path2;
		export_path2 << "C:/Users/Mizuno/Desktop/EvaluationResults/" << simulation_start_time;
		export_path2 << "/" << requirement.dummy_num << "-" << (int)requirement.required_anonymous_area << "-" << (int)(requirement.required_preference_conservation * 100);
		export_path2 << "/confusion_ratio2.csv";

		std::ofstream out_file2(export_path2.str());
		for (std::vector<double>::const_iterator iter = confusion_achive_ratio_with_semantics_vector.begin(); iter != confusion_achive_ratio_with_semantics_vector.end(); iter++) {
			out_file2 << *iter << std::endl;
		}
		out_file2.close();
	}


	///<summary>
	/// 真の嗜好と可観測な嗜好のヒストグラムをエクスポートする
	///</summary>
	void DeimSimulator::export_preference_histogram(const Requirement::PreferenceRequirement& requirement)
	{
		std::stringstream export_path;
		export_path << "C:/Users/Mizuno/Desktop/EvaluationResults/" << simulation_start_time;
		export_path << "/" << requirement.dummy_num << "-" << (int)requirement.required_anonymous_area << "-" << (int)(requirement.required_preference_conservation * 100);
		export_path << "/preference-histogram.csv";

		std::ofstream out_file(export_path.str());

		//Sequenceのキーを作る
		std::set<std::string> sequences;
		std::unordered_map<std::string, double> user_preference_supports;
		std::unordered_map<std::string, double> observed_preference_supports;

		int max_depth = user_preference_tree->max_depth();
		for (int depth = 1; depth <= max_depth; depth++) {
			user_preference_tree->for_each_prefix(depth, [&](const Collection::Sequence<User::category_id>& prefix, double support) {
				std::stringstream key;
				for (Collection::Sequence<User::category_id>::const_iterator iter = prefix.begin(); iter != prefix.end(); iter++) {
					key << *iter;
				}
				sequences.insert(key.str());
				user_preference_supports.insert(std::make_pair(key.str(), support));
			});
		}

		max_depth = observed_preference_tree->max_depth();
		for (int depth = 1; depth <= max_depth; depth++) {
			observed_preference_tree->for_each_prefix(depth, [&](const Collection::Sequence<User::category_id>& prefix, double support) {
				std::stringstream key;
				for (Collection::Sequence<User::category_id>::const_iterator iter = prefix.begin(); iter != prefix.end(); iter++) {
					key << *iter;
				}
				sequences.insert(key.str());
				observed_preference_supports.insert(std::make_pair(key.str(), support));
			});
		}

		for (std::set<std::string>::const_iterator key = sequences.begin(); key != sequences.end(); key++) {
			auto temp1 = user_preference_supports.find(*key);
			auto temp2 = observed_preference_supports.find(*key);
			double user_support = temp1 == user_preference_supports.end() ? 0.0 : temp1->second;
			double observed_support = temp2 == observed_preference_supports.end() ? 0.0 : temp2->second;
			out_file << *key << "," << user_support << "," << observed_support << std::endl;
		}

		out_file.close();
	}

}


