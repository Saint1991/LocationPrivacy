#ifdef SIMULATOR_EXPORTS
#define HAYASHIDA_SIMULATOR_API __declspec(dllexport)
#else
#define HAYASHIDA_SIMULATOR_API __declspec(dllimport)
#endif

#pragma once
#include "HayashidaDbMap.h"
#include "LatLng.h"
#include "RevisablePauseMobileEntity.h"
#include "KatoMethodRequirement.h"
#include "ISimulator.h"
#include "KatoMasterMethod.h"
#include "Rectangle.h"
#include "Probability.h"
#include "GeoCalculation.h"
#include "RevisableTrajectory.h"
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
		: public ISimulator<Map::HayashidaDbMap, Entity::DifferentMovementUser<Geography::LatLng>, Entity::RevisablePauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement, Geography::LatLng, Graph::RevisableTrajectory<Geography::LatLng>>
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
		static constexpr int POI_NUM = 3;
		static constexpr double BASE_LAT = 35.655;//�o���n�̖ڈ��̈ܓx�D�SPOI�̕��ϒl
		static constexpr double BASE_LNG = 139.700;//�o���n�̖ڈ��̌o�x�D�SPOI�̕��ϒl
		int end_time = 2250;//�ڈ��̏I�����ԁD�o�H��ݒ肵�I���āC���Ԃ��]�������͍폜�D
		double length_of_rect = 0.005;//�ЂƂ���POI��I�Ԏ��͈̔́D�K�؂Ȕ͈͓��̈ܓx�o�x��������.
		double length_of_all_POI_rect = 0.05;
	#pragma endregion �v���p�����[�^(�_�~�[��requirement�Ɠ����ɂ��邱��)
	
	protected:
		//�����o�ϐ�
		Graph::Rectangle<Geography::LatLng> map_boundary;
		std::vector<std::shared_ptr<Map::BasicPoi const>> input_poi_list;

		//���\�b�h
		std::vector<std::shared_ptr<Map::BasicPoi const>> get_pois_list(Graph::Rectangle<Geography::LatLng>& boundary);
		void set_path_between_poi(std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator& now_poi, std::vector<Graph::MapNodeIndicator>::iterator& path_iter, Graph::MapNodeIndicator& nearest_position, double pause_position_speed, double service_interval, double *distance, int *phase_id);
		std::shared_ptr<Time::TimeSlotManager> create_time_manager();
		void set_pause_time_and_phases_of_visited_POI(int *phase_id, double rest_pause_time, int total_pause_phase, std::vector<std::shared_ptr<Map::BasicPoi const>>::iterator& now_poi);
		
		void input_visit_pois();
		std::vector<std::vector<std::shared_ptr<Map::BasicPoi const>>> all_traveling_salesman_problem(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois);
		std::vector<std::shared_ptr<Map::BasicPoi const>> traveling_salesman_problem(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois);
		std::vector<std::shared_ptr<Map::BasicPoi const>> the_power_alpha_of_the_reciprocal_of_the_ratio_of_the_optimal_distance(std::vector<std::shared_ptr<Map::BasicPoi const>>& visited_pois, int alpha);

		void make_random_movement_user();//�S�v�f�̓��͂�O��
		void make_predicted_user();
		void make_real_user();
		
		void export_dummy_trajectory(std::shared_ptr<Entity::EntityManager<Entity::RevisablePauseMobileEntity<Geography::LatLng>, Entity::DifferentMovementUser<Geography::LatLng>, Geography::LatLng>> entities, std::shared_ptr<Time::Timer> timer, int dummy_id);
		void export_dummies_trajectory(std::shared_ptr<Entity::EntityManager<Entity::RevisablePauseMobileEntity<Geography::LatLng>, Entity::DifferentMovementUser<Geography::LatLng>, Geography::LatLng>> entities, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::Timer> timer);
		void export_MTC();


		void build_map(const Graph::Rectangle<Geography::LatLng>& boundary);
		void create_trajectories();
		void make_requirement_list();
		
	public:
		HayashidaSimulator(const Graph::Rectangle<Geography::LatLng>& map_boundary);
		virtual ~HayashidaSimulator();
		void prepare();
		void run();
		void evaluate();
		void export_evaluation_result();
		
	};

}
