#include "stdafx.h"
#include "TimeSlotManager.h"

namespace Time 
{

	///<summary>
	/// �R���X�g���N�^
	/// UNIX_TIMESTAMP�̌n���n���Čo�ߎ����̃��X�g�ɕϊ�����
	/// relative_to_first��false�ɂ���Ɛ���UNIX_TIMESTAMP�̒l�����̂܂ܕێ�����
	///</summary>
	TimeSlotManager::TimeSlotManager(std::unique_ptr<std::vector<time_t>> timeslots, bool relative_to_first)
	{
		std::sort(timeslots->begin(), timeslots->end());
		if (!relative_to_first) this->timeslots = std::move(timeslots);
		else {
			this->timeslots = std::make_unique<std::vector<time_t>>(timeslots->size());
			if (timeslots->size() == 0) return;
			
			time_t first = timeslots->at(0);
			for (int i = 0; i < timeslots->size(); i++) {
				this->timeslots->at(i) = timeslots->at(i) - first;
			}
		}
	}


	///<summary>
	/// �R���X�g���N�^
	/// �������TIMESTAMP�𗘗p����ꍇ
	/// �s���Ȓl�͏��������̂Œ���
	///</summary>
	TimeSlotManager::TimeSlotManager(std::unique_ptr<std::vector<std::string>> timeslots, bool relative_to_first)
	{
		std::unique_ptr<std::vector<time_t>> unix_timeslots = std::make_unique<std::vector<time_t>>();
		for (int i = 0; i < timeslots->size(); i++) {
			time_t time = TimeUtility::convert_to_unixtimestamp(timeslots->at(i));
			unix_timeslots->push_back(time);
		}
		
		std::sort(unix_timeslots->begin(), unix_timeslots->end());
		if (!relative_to_first) this->timeslots = std::move(unix_timeslots);
		else {
			this->timeslots = std::make_unique<std::vector<time_t>>(unix_timeslots->size());
			if (this->timeslots->size() == 0) return;

			time_t first = unix_timeslots->at(0);
			for (int i = 0; i < unix_timeslots->size(); i++) {
				this->timeslots->at(i) = unix_timeslots->at(i) - first;
			}
		}
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	TimeSlotManager::~TimeSlotManager()
	{
	}

	///<summary>
	/// �e�����ɂ���execute_function�����s����
	/// �����͎���, 1�O��Phase����̌o�ߎ��ԁC���݂�Phase
	///</summary>
	void TimeSlotManager::for_each_time(const std::function<void(time_t, long, int)>& execute_function) const
	{
		execute_function(timeslots->at(0), 0, 0);
		for (int phase = 1; phase < timeslots->size(); phase++) {
			long duration = timeslots->at(phase) - timeslots->at(phase - 1);
			execute_function(timeslots->at(phase), duration, phase);
		}
	}


	///<summary>
	/// �w�肵��Phase�ɂ����鎞�����擾����
	///</summary>
	time_t TimeSlotManager::time_of_phase(int phase) const {
		return timeslots->at(phase);
	}


	///<summary>
	/// �ŏIPhase���擾����
	///</summary>
	int TimeSlotManager::last_phase() const 
	{
		return timeslots->size() - 1;
	}
}

