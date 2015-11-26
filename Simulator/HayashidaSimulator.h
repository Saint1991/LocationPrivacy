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
#include "MTCCalculationModule.h"
#include "TimeUtility.h"

namespace Simulation
{
	///<summary>
	/// �V�~�����[�^����
	///</summary>
	class HAYASHIDA_SIMULATOR_API HayashidaSimulator 
		: public ISimulator<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement, Geography::LatLng, Graph::Trajectory<Geography::LatLng>>
	{
	private:
	#pragma region
		//�ڈ�
		//����300m~1000m�CPOI3��,service_interval360��15phase���炢
		static constexpr auto USER_TRAJECTORY_OUT_PATH = "C:/Users/Shuhei/Desktop/Result_Path/user_trajectory";
		static constexpr auto DUMMY_TRAJECTORT_OUT_PATH = "C:/Users/Shuhei/Desktop/Result_Path/dummy_trajectory";
		std::stringstream export_path;
		//export_path << "C:/Users/Shuhei/Desktop/EvaluationResults/" << Time::TimeUtility::current_timestamp() << "/";
				
		static constexpr double AVERAGE_SPEED = 1.5;
		static constexpr double RANGE_OF_SPEED = 0.5;
		static constexpr int MAX_PAUSE_TIME = 600;//�r���ړI�n�ݒ�̊֌W��min_pause_time��2.5�{�ȏ�Őݒ肷��I
		static constexpr int MIN_PAUSE_TIME = 200;//SERVICE_INTERVAL���傫������I
		static constexpr int SERVICE_INTERVAL = 90;
		static constexpr int POI_NUM = 15;
		static constexpr double BASE_LAT = 35.655;//�o���n�̖ڈ��̈ܓx�D�SPOI�̕��ϒl
		static constexpr double BASE_LNG = 139.700;//�o���n�̖ڈ��̌o�x�D�SPOI�̕��ϒl
		int end_time = 18000;//�ڈ��̏I�����ԁD�o�H��ݒ肵�I���āC���Ԃ��]�������͍폜�D
		double length_of_rect = 0.005;//�����ɂ͓K�؂Ȕ͈͓��̈ܓx�o�x��������
	#pragma endregion �v���p�����[�^(�_�~�[��requirement�Ɠ����ɂ��邱��)
	
	protected:
		Graph::Rectangle<Geography::LatLng> map_boundary;
		std::vector<std::shared_ptr<Map::BasicPoi const>> get_pois_list(Graph::Rectangle<Geography::LatLng>& boundary);
		void set_path_between_poi(std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator& now_poi, std::vector<Graph::MapNodeIndicator>::iterator& path_iter, Graph::MapNodeIndicator& nearest_position, double pause_position_speed, double service_interval, double *distance, int *phase_id);
		
		void build_map(const Graph::Rectangle<Geography::LatLng>& boundary);
		void create_trajectories();
		void make_requirement_list();
		void random_user();

		void export_dummy_trajectory(std::shared_ptr<Entity::EntityManager<Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Geography::LatLng>> entities, std::shared_ptr<Time::Timer> timer, int dummy_id);
		void export_dummies_trajectory(std::shared_ptr<Entity::EntityManager<Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Geography::LatLng>> entities, std::shared_ptr<Time::Timer> timer);

		void export_MTC();

	public:
		HayashidaSimulator(const Graph::Rectangle<Geography::LatLng>& map_boundary);
		virtual ~HayashidaSimulator();
		void prepare();
		void run();
		void evaluate();
		void export_evaluation_result();
		
	};

}
