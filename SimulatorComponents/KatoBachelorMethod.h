#ifdef SIMULATORCOMPONENTS_EXPORTS
#define KATO_BACHELOR_METHOD_API __declspec(dllexport)
#else
#define KATO_BACHELOR_METHOD_API __declspec(dllimport)
#endif
#pragma once
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "IProposedMethod.h"
#include "Rectangle.h"
#include "RevisablePauseMobileEntity.h"
#include "KatoMethodRequirement.h"
#include "Probability.h"
#include "GeoCalculation.h"
#include "RevisableTrajectory.h"
namespace Method
{

	///<summary>
	/// 加藤さん学部卒業論文の提案手法の実装
	///</summary>
	class KATO_BACHELOR_METHOD_API KatoBachelorMethod
		: public Framework::IProposedMethod<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::RevisablePauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement, Geography::LatLng, Graph::RevisableTrajectory<Geography::LatLng>>
	{
	private:
		//変数
		const int CELL_NUM_ON_SIDE = 3;//グリッドのセルの一辺の数

		typedef std::vector<Graph::Rectangle<Geography::LatLng>> Grid;
		std::vector<Grid> grid_list;

		typedef unsigned int entity_id;

		static constexpr auto DUMMY_TRAJECTORT_OUT_PATH = "C:/Users/Shuhei/Desktop/dummy_trajectory";
		
		std::shared_ptr<Entity::RevisablePauseMobileEntity<Geography::LatLng>> creating_dummy;
		
		//メソッド
		std::vector<Graph::Rectangle<Geography::LatLng>> make_grid(double grid_area, const Geography::LatLng& center, int cell_num_on_side);//Gridの作成
		std::vector<int> get_total_num_of_each_cell_at_interval_phase(std::vector<std::vector<int>>& entities_num_table, int start, int end);//各セルのフェーズトータルのエンティティの数
		std::vector<int> get_total_num_of_each_cell_at_all_phase(std::vector<std::vector<int>>& entities_num_table);//各セルのフェーズトータルのエンティティの数
		std::list<std::pair<int, int>> get_cell_id_list_order_by_entities_num(std::vector<int>& total_entity_num) const;
		std::list<std::pair<int, int>> get_phase_of_min_cell_id_list_ordered_by_entities_num(std::vector<std::vector<int>>& entities_num_table, int min_cell_id, int start, int end) const;
		std::vector<std::shared_ptr<Map::BasicPoi const>> get_candidate_pois_list(Graph::Rectangle<Geography::LatLng>& boundary);
		void set_pause_time_and_speed_0_of_visitedPOI(int *phase_id, double rest_pause_time, int total_pause_phase, const Graph::MapNodeIndicator& source);
		void linear_interpolation_of_path_between_positions(const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int *phase_id, double *dest_rest_time);
		void decide_base_positions_and_arrive_time(int dummy_id);// 生成中ダミー(k番目)の基準地点および基準地点到着時間の決定
		void decide_share_positions_and_arrive_time(int dummy_id);// 生成中ダミー(k番目)の共有地点および共有地点到着時間の決定
		void decide_destination_on_the_way(int dummy_id);// 生成中ダミー(k番目)の移動経路の決定
	
	protected:
		void initialize();
		void decide_dummy_positions();
		
	public:
		KatoBachelorMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~KatoBachelorMethod();

		void run();

	};
}


