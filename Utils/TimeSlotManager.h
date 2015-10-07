#ifdef UTILS_EXPORTS
#define TIMESLOT_MANAGER_API __declspec(dllexport)
#else
#define TIMESLOT_MANAGER_API __declspec(dllimport)
#endif

#pragma once
#include "TimeUtility.h"

namespace Time
{

	///<summary>
	/// �T�[�r�X���p�̎�������Ǘ�����N���X
	/// Phase�͉��Ԗڂ̃^�C���X���b�g��
	/// time�͂���Phase���\������
	///</summary>
	class TIMESLOT_MANAGER_API TimeSlotManager
	{
	protected:
		std::unique_ptr<std::vector<time_t>> timeslots;

	public:
		TimeSlotManager(std::unique_ptr<std::vector<time_t>> timeslots, bool relative_to_first = true);
		TimeSlotManager(std::unique_ptr<std::vector<std::string>> timeslots, bool relative_to_first = true);
		~TimeSlotManager();

		void for_each_time(const std::function<void(time_t, long, int)>& execute_function) const;
		time_t time_of_phase(int phase) const;
		int find_phase_of_time(time_t time) const;
		size_t phase_count() const;
	};
}


