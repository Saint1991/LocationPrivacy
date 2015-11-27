#ifdef SIMULATORCOMPONENTS_EXPORTS
#define PAUSE_MOBILE_ENTITY_API __declspec(dllexport)
#else
#define PAUSE_MOBILE_ENTITY_API __declspec(dllimport)
#endif

#pragma once
#include "MobileEntity.h"
#include "KatoMethodRequirement.h"

namespace Entity
{

	typedef unsigned int entity_id;

	///<summary>
	/// ��~���Ԃ��܂񂾈ړ��̂�\���N���X
	/// ���[�U�C�_�~�[��\���̂ɗp����N���X (MobileEntity����h��)
	///</summary>
	template <typename POSITION_TYPE = Geography::LatLng>
	class PAUSE_MOBILE_ENTITY_API PauseMobileEntity : public MobileEntity<POSITION_TYPE>
	{
	protected:
		std::vector<double> pause_time_list;
		std::vector<double> speed_list;
		std::vector<std::pair<std::vector<int>, std::pair<Graph::MapNodeIndicator, Geography::LatLng>>> pause_poi_info;//<poi_info,position>
		std::vector<std::pair<int,double>> pause_time_info;//<phase,rest_pause_time>

	public:
		PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot);
		virtual ~PauseMobileEntity();

		int get_pause_time(int phase) const;
		void set_pause_time(int phase, int pause_time);
		void set_pause_time(int phase, double pause_time);
		void set_random_pause_time(int phase, int min, int max);
		void set_random_pause_time(int phase, double min, double max);

		double get_speed(int phase) const;
		void set_speed(int phase, double speed);
		void set_random_speed(int phase, double average_speed, double speed_range);
		
		//poi_info��get��set���郁�\�b�h���쐬����
		std::vector<std::pair<std::vector<int>, std::pair<Graph::MapNodeIndicator, Geography::LatLng>>> get_pause_poi_info();

		std::vector<int> find_cross_not_set_phases_of_poi() const;
		int randomly_pick_cross_not_set_phase_of_poi() const;
	};

	//�����I���ꉻ
	template class PauseMobileEntity<Graph::Coordinate>;
	template class PauseMobileEntity<Geography::LatLng>;
}