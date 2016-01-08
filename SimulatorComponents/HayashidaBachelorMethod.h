#ifdef SIMULATORCOMPONENTS_EXPORTS
#define HAYASHIDA_BACHELOR_METHOD_API __declspec(dllexport)
#else
#define HAYASHIDA_BACHELOR_METHOD_API __declspec(dllimport)
#endif


#pragma once
#include "IProposedMethod.h"
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "Rectangle.h"
#include "PauseMobileEntity.h"
#include "KatoMethodRequirement.h"
#include "Probability.h"
#include "GeoCalculation.h"
#include "KatoMasterMethod.h"

namespace Method {

	///<summary>
	/// 林田さん学部卒業論文の提案手法の実装
	///</summary>
	class HAYASHIDA_BACHELOR_METHOD_API HayashidaBachelorMethod : public KatoMasterMethod
	{
	protected:
		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> predicted_user;
		std::vector<std::shared_ptr<Map::BasicPoi const>> input_poi_list;

		bool is_user_moving_to_POI();
		Graph::MapNodeIndicator predict_next_poi();
		void repredicting_user_trajectory(int now_phase);
		void last_behavior_of_predicted_user(int phase_id, double dest_rest_time, Graph::MapNodeIndicator& now_poi);
			void pause_until_last_phase(int phase_id, double dest_rest_time, Graph::MapNodeIndicator& now_poi);
			void go_to_random_position(int phase_id, double dest_rest_time, Graph::MapNodeIndicator& now_poi);
		void re_setting_of_user_cross(int now_phase);

		void calc_dummy_path(entity_id target_dummy_id, const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int *phase_id, double *dest_rest_time, double speed);
		void set_now_pause_time_and_phase(entity_id target_dummy_id, Graph::MapNodeIndicator& target_pos, Graph::MapNodeIndicator& next_pos, int *phase_id, double *dest_rest_time, int next_phase, int target_phase, double speed);

		std::vector<std::pair<std::pair<entity_id, int>, double>> calc_distance_between_user_and_dummies(std::vector<int> all_pause_phases);
		std::vector<Graph::MapNodeIndicator> repredict_visited_poi_order_by_tsp(std::vector<Graph::MapNodeIndicator>& input_poi_list);
		
		void set_predicted_user_path(std::vector<Graph::MapNodeIndicator>::iterator& now_poi, std::vector<Graph::MapNodeIndicator>::iterator& path_iter, Graph::MapNodeIndicator& nearest_position, double pause_position_speed, double service_interval, double *distance, int *phase_id);
		void set_predicted_user_pause_time_and_phases(int *phase_id, double rest_pause_time, int total_pause_phase, Graph::MapNodeIndicator now_poi);
		
		
		//void initialize();
		void adjust_dummy_positions();
		//void evaluate();
		//void export_results();
		//void terminate();
		
	public:
		HayashidaBachelorMethod(std::shared_ptr<Map::HayashidaDbMap const> map, std::shared_ptr<Entity::DifferentMovementUser<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~HayashidaBachelorMethod();

		void run();
	};

}
