#include "stdafx.h"
#include "TimeSlotManager.h"

namespace Time 
{

	///<summary>
	/// �R���X�g���N�^
	/// UNIX_TIMESTAMP�̌n���n���Čo�ߎ����̃��X�g�ɕϊ�����
	/// relative_to_first��false�ɂ���Ɛ���UNIX_TIMESTAMP�̒l�����̂܂ܕێ�����
	///</summary>
	TimeSlotManager::TimeSlotManager(std::unique_ptr<std::vector<time_t>> times, bool relative_to_first) : timeslots(std::make_unique<std::vector<std::vector<time_t>>>())
	{
		if (times->size() == 0) return;
		
		std::sort(times->begin(), times->end());
		
		time_t first = times->at(0);
		std::string date = TimeUtility::date(first);
		std::vector<time_t> time_of_a_trajectory;
		for (std::vector<time_t>::const_iterator iter = times->begin(); iter != times->end(); iter++) {
			std::string target_date = TimeUtility::date(*iter);
			if (target_date != date) {
				date = target_date;
				first = *iter;
				timeslots->push_back(time_of_a_trajectory);
				time_of_a_trajectory = std::vector<time_t>();
			}
			time_of_a_trajectory.push_back(relative_to_first ? *iter - first : *iter);
		}
	}


	///<summary>
	/// �R���X�g���N�^
	/// �������TIMESTAMP�𗘗p����ꍇ
	/// �s���Ȓl�͏��������̂Œ���
	///</summary>
	TimeSlotManager::TimeSlotManager(std::unique_ptr<std::vector<std::string>> times, bool relative_to_first) : timeslots(std::make_unique<std::vector<std::vector<time_t>>>())
	{
		if (times->size() == 0) return;
		
		std::unique_ptr<std::vector<time_t>> unix_timeslots = std::make_unique<std::vector<time_t>>();
		for (int i = 0; i < times->size(); i++) {
			time_t time = TimeUtility::convert_to_unixtimestamp(times->at(i));
			unix_timeslots->push_back(time);
		}
		
		std::sort(unix_timeslots->begin(), unix_timeslots->end());

		time_t first = unix_timeslots->at(0);
		std::string date = TimeUtility::date(first);
		std::vector<time_t> time_of_a_trajectory;
		for (std::vector<time_t>::const_iterator iter = unix_timeslots->begin(); iter != unix_timeslots->end(); iter++) {
			std::string target_date = TimeUtility::date(*iter);
			if (target_date != date) {
				date = target_date;
				first = *iter;
				timeslots->push_back(time_of_a_trajectory);
				time_of_a_trajectory = std::vector<time_t>();
			}
			time_of_a_trajectory.push_back(relative_to_first ? *iter - first : *iter);
		}
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	TimeSlotManager::~TimeSlotManager()
	{
	}

	///<summary>
	/// ������Date-phase�͈قȂ���t�ʂŉ��Ԗڂ̃g���W�F�N�g����������\��
	/// �e�����ɂ���execute_function�����s����
	/// �����͎���, 1�O��Phase����̌o�ߎ��ԁC���݂�Phase
	///</summary>
	void TimeSlotManager::for_each_time(const std::function<void(time_t, long, int)>& execute_function, int date_phase) const
	{
		if (timeslots->size() == 0) return;
		if (timeslots->at(date_phase).size() == 0) return;

		execute_function(timeslots->at(date_phase).at(0), 0, 0);
		for (int phase = 1; phase < timeslots->at(date_phase).size(); phase++) {
			long duration = timeslots->at(date_phase).at(phase) - timeslots->at(date_phase).at(phase - 1);
			execute_function(timeslots->at(date_phase).at(phase), duration, phase);
		}
	}


	///<summary>
	/// �w�肵��Phase�ɂ����鎞�����擾����
	///</summary>
	time_t TimeSlotManager::time_of_phase(int phase, int date_phase) const 
	{
		return timeslots->at(date_phase).at(phase);
	}


	///<summary>
	/// time�����Ƃ�phase���擾���܂�
	/// �l��time�̃G���g�������݂��Ȃ��ꍇ��INVALID=-1��Ԃ��܂�
	///</summary>
	int TimeSlotManager::find_phase_of_time(time_t time, int date_phase) const
	{
		std::vector<time_t>::iterator iter = std::find(timeslots->at(date_phase).begin(), timeslots->at(date_phase).end(), time);
		size_t index = std::distance(timeslots->at(date_phase).begin(), iter);
		if (index == timeslots->at(date_phase).size()) return INVALID;
		return index;
	}


	///<summary>
	/// �ŏIPhase���擾����
	///</summary>
	size_t TimeSlotManager::phase_count(int date_phase) const 
	{
		return timeslots->at(date_phase).size();
	}


	///<summary>
	/// �������g���W�F�N�g�������擾���� 
	///</summary>
	size_t TimeSlotManager::date_phase_count()const
	{
		return timeslots->size();
	}
}

