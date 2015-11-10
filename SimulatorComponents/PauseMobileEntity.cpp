#include "stdafx.h"
#include "PauseMobileEntity.h"


namespace Entity
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	PauseMobileEntity<POSITION_TYPE>::PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: MobileEntity<POSITION_TYPE>(id, timeslot), 
		  pause_time_list(std::vector<int>(timeslot->phase_count(),0)), 
		  speed_list(std::vector<double>(timeslot->phase_count(), 0))
	{
		
	}

	///<summary>
	/// デストラクタ
	///</summary>
	template <typename POSITION_TYPE>
	PauseMobileEntity<POSITION_TYPE>::~PauseMobileEntity()
	{
	}


	///<summary>
	/// 停止時間を求める
	///</summary>
	template <typename POSITION_TYPE>
	int PauseMobileEntity<POSITION_TYPE>::get_pause_time(int phase) const
	{
		return pause_time_list.at(phase);
	}

	///<summary>
	/// 特定の値を停止時間としてsetする
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_pause_time(int phase, time_t pause_time)
	{
		pause_time_list.at(phase) = pause_time;
	}

	
	///<summary>
	/// ランダムな値を停止時間としてsetする
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_random_pause_time(int phase, int min, int max)
	{
		Math::Probability generator;
		int pause_time = generator.uniform_distribution(min, max);
		
		pause_time_list.at(phase) = pause_time;
	}

	///<summary>
	/// 移動速度を求める
	///</summary>
	template <typename POSITION_TYPE>
	double PauseMobileEntity<POSITION_TYPE>::get_speed(int phase) const
	{
		return speed_list.at(phase);
	}

	///<summary>
	/// 特定の値の移動速度をsetする
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_speed(int phase, double speed)
	{
		speed_list.at(phase) = speed;
	}

	

	///<summary>
	/// ランダムな値の移動速度をsetする
	///</summary>
	template <typename POSITION_TYPE>
	void PauseMobileEntity<POSITION_TYPE>::set_random_speed(int phase, double average_speed, double speed_range)
	{
		Math::Probability generator;
		double min = average_speed -0.5* speed_range;
		double max = average_speed + 0.5* speed_range;
		double dummy_speed = generator.uniform_distribution(min, max);

		speed_list.at(phase) = dummy_speed;
	}
	
	///<summary>
	/// startからendまでの間でrandomにphaseをsetする
	///</summary>
	template <typename POSITION_TYPE>
	int PauseMobileEntity<POSITION_TYPE>::get_random_phase(int start, int end)
	{
		Math::Probability generator;
		int random_phase = generator.uniform_distribution(start, end);

		return random_phase;

	}

}
