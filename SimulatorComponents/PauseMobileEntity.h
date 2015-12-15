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

	class VisitedPoiInfo
	{
	public:
		VisitedPoiInfo();
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
		int pause_flag;

		std::vector<double> now_speed_list;
		
	public:
		PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		virtual ~PauseMobileEntity();
		
		//�K��POI(visited_poi)�Ɋւ���setter��getter
		void set_visited_poi_of_phase(int phase, const Graph::MapNodeIndicator& node_id, const Geography::LatLng& position);
		void set_crossing_position_of_phase(int phase, const Graph::MapNodeIndicator& node_id, Geography::LatLng position, const std::string& venue_name = "");//MobileEntity�̃I�[�o�[���C�h
		std::pair<Graph::MapNodeIndicator, Geography::LatLng> get_poi();
		VisitedPoiInfo get_next_poi_info();
		void clear_visited_poi_info();

		//pause_phase�Ɋւ���setter��getter
		std::vector<int> get_pause_phases(int phase);
		void set_pause_phases(int pause_phase);

		//arrive_phase�Ɋւ���setter��getter.�������Cset��pause_time�Ɠ����ɐݒ���s�����ߕs�v�D
		int get_arrive_phase();
		

		//�K��POI���̒�~����(pause_time)�Ɋւ���setter��getter
		int get_pause_time() const;
		int get_pause_time();
		void set_pause_time(int phase, int pause_time);
		void set_pause_time(int phase, double pause_time);
		void set_random_pause_time(int phase, int min, int max);
		void set_random_pause_time(int phase, double min, double max);
		
		//starting_speed�Ɋւ���setter��getter
		void set_starting_speed_at_poi(double speed);
		void set_random_starting_speed_at_poi(double average_speed, double speed_range);
		double get_starting_speed();


		//����phase�ɂ�����c���~���Ԃ�setter��getter
		void set_rest_pause_time_when_departing(double rest_pause_time);


		//�������Əo�����̗]�莞�Ԃ�setter��getter
		void set_dest_rest_time(double dest_rest_time);
		

		//���ݏ���rest_pause_time�Ɋւ���setter��getter
		int get_now_pause_time(int now_phase) const;
		int get_now_pause_time(int now_phase);
		void set_now_pause_time(int now_phase, double time);



		//����phase�ɂ����鑬�x��setter��getter
		double get_now_speed(int phase) const;
		void set_now_speed(int phase, double speed);
		void set_random_now_speed(int phase, double average_speed, double speed_range);
				

		//��~POI�ɂ��邩�ǂ����̃`�F�b�N�p
		bool check_pause_flag();
		void raise_flag();
		void take_down_flag();

		void increment_visited_pois_info_list_id();

		//trajectory�擾�p
		std::shared_ptr<TRAJECTORY_TYPE> get_trajectory();
		
		
		std::vector<int> find_cross_not_set_phases_of_poi() const;
		int randomly_pick_cross_not_set_phase_of_poi() const;

	};

	//�����I���ꉻ
	template class PauseMobileEntity<Graph::Coordinate>;
	template class PauseMobileEntity<Geography::LatLng>;
}