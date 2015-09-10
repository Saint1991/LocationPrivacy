#include "stdafx.h"
#include "Timer.h"

namespace Time
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	Timer::Timer() : start_time(INVALID), end_time(INVALID)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	Timer::~Timer()
	{
	}


	///<summary>
	///�v�����J�n����
	///</summary>
	void Timer::start()
	{
		time(&start_time);
	}


	///<summary>
	/// �v�����I������
	///</summary>
	void Timer::end()
	{
		time(&end_time);
	}


	///<summary>
	/// �v�����I�����Ă��邩��Ԃ�
	///</summary>
	bool Timer::is_finished() const
	{
		return start_time != INVALID && end_time != INVALID;
	}


	///<summary>
	/// �o�ߎ���[sec]��Ԃ�
	///</summary>
	time_t Timer::duration() const
	{
		if (start_time == INVALID || end_time == INVALID) return INVALID;
		return end_time - start_time;
	}


	///<summary>
	/// �o�ߎ�����P�ʕt��������ŕԂ�
	///</summary>
	const std::string Timer::to_string() const
	{
		if (!is_finished()) return "NOT FINISHED";
		return std::to_string(duration()) + "[sec]";
	}

	#pragma region Export
	const std::string Timer::DURATION = "Duration";


	///<summary>
	/// Export�p�f�[�^�̍쐬
	///</summary>
	std::unordered_map<std::string, std::string> Timer::get_export_data()  const
	{
		std::unordered_map<std::string, std::string> data = {
			{DURATION, std::to_string(duration())}
		};
		return data;
	}
	#pragma endregion
}

