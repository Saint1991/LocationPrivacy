#ifdef UTILS_EXPORTS
#define KATOMETHODREQUIREMENT_API __declspec(dllexport)
#else
#define KATOMETHODREQUIREMENT_API __declspec(dllimport)
#endif

#pragma once
#include "BasicRequirement.h"

namespace Requirement {

	///<summary>
	/// ������@�ɕK�v��Requirement
	///</summary>
	class KATOMETHODREQUIREMENT_API KatoMethodRequirement : public Requirement::BasicRequirement
	{
	public:
		//�������񑲘_��@�E�C�_��@�ɋ��ʂȃp�����[�^
		int max_pause_time;
		int min_pause_time;
				
		int interval_of_base_phase;//�����̈�m�ۂ̂��߂̊�n�_�����肷��Ԋu
		int cycle_of_interval_of_base_phase;//�����̈�m�ۂ̂��߂̊�n�_�����肷��Ԋu�̎���

		int service_interval;
		

		//��������C�_��@�݂̂ɕK�v�ȃp�����[�^
		int max_variation_of_pause_time;
		double max_variation_of_speed;

		
		KatoMethodRequirement();
		
		//�������񑲘_��@�̃R���X�g���N�^
		KatoMethodRequirement(
			double required_anonymous_area, 
			size_t dummy_num, 
			int service_interval, 
			int interval_of_base_phase, 
			int cycle_of_interval_of_base_phase, 
			int max_pause_time = 600, 
			int min_pause_time = 60, 
			double average_speed = 3.0, 
			double speed_range = 2.0
		);
		
		//��������C�_��@�̃R���X�g���N�^
		KatoMethodRequirement(
			double required_anonymous_area,
			size_t dummy_num,
			int service_interval,
			int max_variation_of_pause_time,
			double max_variation_of_speed,
			int interval_of_base_phase,
			int cycle_of_interval_of_base_phase,
			int max_pause_time = 600,
			int min_pause_time = 60,
			double average_speed = 3.0,
			double speed_range = 2.0
		);

		virtual ~KatoMethodRequirement();
	
	};
}
