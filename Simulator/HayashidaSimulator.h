#ifdef SIMULATOR_EXPORTS
#define HAYASHIDA_SIMULATOR_API __declspec(dllexport)
#else
#define HAYASHIDA_SIMULATOR_API __declspec(dllimport)
#endif

#pragma once
#include "BasicDbMap.h"
#include "LatLng.h"
#include "PauseMobileEntity.h"
#include "KatoMethodRequirement.h"
#include "ISimulator.h"
#include "KatoMasterMethod.h"
#include "Rectangle.h"
#include "Probability.h"
#include "GeoCalculation.h"
#include "Trajectory.h"
#include "Dijkstra.h"
#include "FileExporter.h"

namespace Simulation
{
	///<summary>
	/// シミュレータ実装
	///</summary>
	class HAYASHIDA_SIMULATOR_API HayashidaSimulator 
		: public ISimulator<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement, Geography::LatLng, Graph::Trajectory<Geography::LatLng>>
	{
	private:
	#pragma region
		//目安
		//距離300m~1000m，POI3個,service_interval360で15phaseぐらい
		static constexpr auto USER_TRAJECTORY_OUT_PATH = "C:/Users/Shuhei/Desktop/user_trajectory";
		static constexpr double AVERAGE_SPEED = 1.5;
		static constexpr double RANGE_OF_SPEED = 0.5;
		static constexpr int MAX_PAUSE_TIME = 700;
		static constexpr int MIN_PAUSE_TIME = 500;//SERVICE_INTERVALより大きくする！
		static constexpr int SERVICE_INTERVAL = 90;
		static constexpr int POI_NUM = 2;
		static constexpr double base_lat = 35.655;//出発地の目安の緯度．全POIの平均値
		static constexpr double base_lng = 139.700;//出発地の目安の経度．全POIの平均値
		int end_time = 1800;//目安の終了時間．経路を設定し終えて，時間が余った分は削除．
		double length_of_rect = 0.005;//ここには適切な範囲内の緯度経度差を書く
	#pragma endregion 要求パラメータ(ダミーのrequirementと同じにすること)
	
	protected:
		Graph::Rectangle<Geography::LatLng> map_boundary;
				
		std::vector<std::shared_ptr<Map::BasicPoi const>> get_pois_list(Graph::Rectangle<Geography::LatLng>& boundary);
		void set_path_between_poi(std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator& now_poi, std::vector<Graph::MapNodeIndicator>::iterator& path_iter, Graph::MapNodeIndicator& nearest_position, double pause_position_speed, double service_interval, double *distance, int *phase_id);
		
		void build_map(const Graph::Rectangle<Geography::LatLng>& boundary);
		void create_trajectories();
		void make_requirement_list();
		void random_user();

	public:
		HayashidaSimulator(const Graph::Rectangle<Geography::LatLng>& map_boundary);
		virtual ~HayashidaSimulator();
		void prepare();
		void run();
		void evaluate();
		void export_evaluation_result();
	};

}
