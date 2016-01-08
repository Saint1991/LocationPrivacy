#ifdef SIMULATORCOMPONENTS_EXPORTS
#define PAUSE_MOBILE_ENTITY_API __declspec(dllexport)
#else
#define PAUSE_MOBILE_ENTITY_API __declspec(dllimport)
#endif

#pragma once
#include "MobileEntity.h"
#include "RevisableTrajectory.h"
#include "KatoMethodRequirement.h"
#include "BasicPoi.h"

namespace Entity
{
	typedef unsigned int entity_id;

	class VisitedPoiInfo : public IO::FileExportable
	{
	public:
		static constexpr char* TIME = "time";
		static constexpr char* VENUE_NAME = "venue_name";
		static constexpr char* POI_ID = "poi_id";
		
		std::unordered_map<std::string, std::string> get_export_data() const;
		
		VisitedPoiInfo();
		VisitedPoiInfo(std::pair<Graph::MapNodeIndicator, Geography::LatLng> visited_poi, std::vector<int> pause_phases, int arrive_phase, double pause_time, double starting_speed, double rest_pause_time_when_departing, double dest_rest_time);
		virtual ~VisitedPoiInfo();
		
		//const int POI_NUM = 10;

		std::pair<Graph::MapNodeIndicator, Geography::LatLng> visited_poi;//�K��POI��set_visited_poi_of_phase�Őݒ�
		std::vector<int> pause_phases;//�K��POI�ɂ������~phase��set_pause_phase�Őݒ�
		int arrive_phase;//��������phase��set_random_pause_time��set_pause_time�Őݒ�
		double pause_time;//��~���ԁ�set_random_pause_time��set_pause_time�Őݒ�
		double starting_speed;//�K��POI�̏o�����x��set_starting_speed_at_poi�Őݒ�
		double rest_pause_time_when_departing;//�o�����̗]���~���ԁ�set_rest_pause_time_when_departing�Őݒ�
		double dest_rest_time;//POI�ɓ����������̗]�莞�ԁ�set_rest_time�Őݒ�
	};
	
	///<summary>
	/// ��~���Ԃ��܂񂾈ړ��̂�\���N���X
	/// ���[�U�C�_�~�[��\���̂ɗp����N���X (MobileEntity����h��)
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng, typename TRAJECTORY_TYPE = Graph::RevisableTrajectory<POSITION_TYPE>>
	class PAUSE_MOBILE_ENTITY_API PauseMobileEntity : public MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>
	{
	protected:
		std::vector<VisitedPoiInfo> visited_pois_info_list;
		VisitedPoiInfo visited_poi_info;
		int visited_pois_info_list_id;
		
		std::vector<double> now_pause_time_list;//���ݒn�_�ɂ�����c���~����
		//int pause_flag;

		std::vector<double> now_speed_list;
		
	public:
		PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		virtual ~PauseMobileEntity();
		
		//�S�Ă̖K��POI���Ɋւ���setter��getter
		std::vector<VisitedPoiInfo> get_visited_poi_info_list();

		//�K��POI_ID(visited_pois_info_list_id)�Ɋւ���setter��getter
		void increment_visited_pois_info_list_id();
		int get_visited_pois_info_list_id();
		int get_visited_pois_info_list_id_at_certain_phase(int phase);
		void clear_visited_pois_info_list_id();

		//�K��POI(visited_poi)�Ɋւ���setter��getter
		void set_visited_poi_of_phase(int phase, const Graph::MapNodeIndicator& node_id, const Geography::LatLng& position);
		void set_crossing_position_of_phase(int phase, const Graph::MapNodeIndicator& node_id, Geography::LatLng position, const std::string& venue_name = "");//MobileEntity�̃I�[�o�[���C�h
		std::pair<Graph::MapNodeIndicator, POSITION_TYPE> get_poi();
		std::pair<Graph::MapNodeIndicator, POSITION_TYPE> get_any_visited_poi(int id);
		std::pair<Graph::MapNodeIndicator, POSITION_TYPE> get_next_poi();
		void clear_visited_poi_info();
		void sort_pois_order_by_arrive_phase();
		int get_visited_pois_num();

		//pause_phase�Ɋւ���setter��getter
		std::vector<int> get_pause_phases();
		std::vector<int> get_all_pause_phases_since_current_phase(int current_phase);
		std::vector<int> get_all_pause_phases();
		void set_pause_phases(int pause_phase);
		void set_pause_phases_using_arrive_phase(int arrive_phase, int pause_phase);


		//arrive_phase�Ɋւ���getter.�������Cset��visitedPOI�o�^���ɐݒ���s�����ߕs�v�D
		int get_arrive_phase();
		int get_arrive_phase_using_pause_phase(int pause_phase);
		int get_arrive_phase_of_any_visited_poi(int id);

		//�K��POI���̒�~����(pause_time)�Ɋւ���setter��getter
		double get_pause_time() const;
		double get_pause_time();
		double get_pause_time_using_arrive_phase(int arrive_phase);
		double get_pause_time_of_any_visited_poi(int id);
		void set_pause_time(int phase, int pause_time);
		void set_pause_time(int phase, double pause_time);
		void set_pause_time_using_arrive_phase(int arrive_phase, double pause_time);
		void set_random_pause_time(int phase, int min, int max);
		void set_random_pause_time(int phase, double min, double max);
		void set_random_pause_time_using_arrive_phase(int arrive_phase, int phase, double min, double max);

		//starting_speed�Ɋւ���setter��getter
		void set_starting_speed_at_poi(double speed);
		void set_starting_speed_at_poi_using_arrive_phase(int arrive_phase, double speed);
		void set_random_starting_speed_at_poi(double average_speed, double speed_range);
		void set_random_starting_speed_at_poi_using_arrive_phase(int arrive_phase, double average_speed, double speed_range);
		double get_starting_speed();
		double get_starting_speed_using_arrive_phase(int arrive_phase);
		double get_starting_speed_using_pause_phase(int pause_phase);
		double get_starting_speed_of_any_visited_poi(int id);


		//�o�����̗]�莞�ԂɊւ���setter��getter
		void set_rest_pause_time_when_departing(double rest_pause_time);
		void set_rest_pause_time_when_departing_using_arrive_phase(int arrive_phase, double rest_pause_time);
		double get_rest_pause_time_when_departing_using_pause_phase(int pause_phase);
		double get_rest_pause_time_when_departing_of_any_visited_poi(int id);

		//�������̗]�莞�Ԃ�setter��getter
		void set_dest_rest_time(double dest_rest_time);
		void set_dest_rest_time_using_arrive_phase(int arrive_phase, double dest_rest_time);
		double get_dest_rest_time_of_any_visited_poi(int id);

		//���ݏ���rest_pause_time�Ɋւ���setter��getter
		double get_now_pause_time(int now_phase) const;
		double get_now_pause_time(int now_phase);
		std::vector<double> get_now_pause_time_list();
		void set_now_pause_time(int now_phase, double time);

		//����phase�ɂ����鑬�x��setter��getter
		double get_now_speed(int phase) const;
		void set_now_speed(int phase, double speed);
		void set_random_now_speed(int phase, double average_speed, double speed_range);
				
		//��~POI�ɂ��邩�ǂ����̃`�F�b�N�p
		bool isPause(int now_phase);
		void raise_flag();
		void take_down_flag();

		//trajectory�擾�p
		std::shared_ptr<TRAJECTORY_TYPE> get_trajectory();
		
		//�����񐔔���p
		int get_cross_count_until_the_phase(int phase);
		
		std::vector<int> find_cross_not_set_phases_of_poi() const;
		int randomly_pick_cross_not_set_phase_of_poi() const;

		int get_prev_phase_when_visiting_poi(int phase_id);
		int get_next_phase_when_visiting_poi(int phase_id);
		int get_prev_poi_arrive_phase_when_pausing(int phase_id);
		int get_next_poi_arrive_phase_when_pausing(int phase_id);

		//�t�@�C���o�͗p
		//virtual void foreach(const std::function<void(int, time_t, std::shared_ptr<POSITION_TYPE const>)>& execute_function) const;
		//virtual void foreach_state(const std::function<void(int, long, const Graph::MapNodeIndicator&, std::shared_ptr<TrajectoryState<POSITION_TYPE> const>)>& execute_function) const;
		//virtual std::list<std::shared_ptr<IO::FileExportable const>> get_export_data() const;

	};

	//�����I���ꉻ
	template class PauseMobileEntity<Graph::Coordinate>;
	template class PauseMobileEntity<Geography::LatLng>;
}