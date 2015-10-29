#include "stdafx.h"
#include "SemanticTrajectory.h"

namespace Graph
{


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	SemanticTrajectory<POSITION_TYPE>::SemanticTrajectory(std::unique_ptr<std::vector<time_t>> times, bool use_relative_time)
		: Trajectory<POSITION_TYPE>(std::move(times), use_relative_time)
	{
		category_sequence = std::make_shared<Collection::Sequence<category_id>>(timeslot->phase_count());
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	SemanticTrajectory<POSITION_TYPE>::SemanticTrajectory(std::unique_ptr<std::vector<std::string>> times, bool use_relative_time)
		: Trajectory<POSITION_TYPE>(std::move(times), use_relative_time)
	{
		category_sequence = std::make_shared<Collection::Sequence<category_id>>(timeslot->phase_count());
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	SemanticTrajectory<POSITION_TYPE>::SemanticTrajectory(std::shared_ptr<Time::TimeSlotManager> timeslot) 
		: Trajectory<POSITION_TYPE>(timeslot), category_sequence(std::make_shared<Collection::Sequence<category_id>>(timeslot->phase_count()))
	{

	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	SemanticTrajectory<POSITION_TYPE>::~SemanticTrajectory()
	{
	}


	///<summary>
	/// �w�肵��Phase�ɂ�����J�e�S����ݒ肷��
	///</summary>
	template <typename POSITION_TYPE>
	bool SemanticTrajectory<POSITION_TYPE>::set_category_of_phase(int phase, const std::string& category_id)
	{
		if (phase < 0 || category_sequence->size() <= phase) return false;
		category_sequence->at(phase) = category_id;
	}



	///<summary>
	/// �w�肵�������ɂ�����J�e�S����ݒ肷��
	///</summary>
	template <typename POSITION_TYPE>
	bool SemanticTrajectory<POSITION_TYPE>::set_category_at(time_t time, const std::string& category_id)
	{
		int phase = timeslot->find_phase_of_time(time);
		return set_category_of_phase(phase, category_id);
	}


	///<summary>
	/// �w�肵��Phase�ɂ�����J�e�S��ID��Ԃ�
	/// �s����Phase�̏ꍇ�͋󕶎����Ԃ�
	///</summary>
	template <typename POSITION_TYPE>
	std::string SemanticTrajectory<POSITION_TYPE>::category_of_phase(int phase) const
	{
		if (phase < 0 || category_sequence->size() <= phase) return "";
		return category_sequence->at(phase);
	}


	///<summary>
	/// �w�肵�������ɂ�����J�e�S��ID��Ԃ�
	/// �s���Ȏ����̏ꍇ�͋󕶎����Ԃ�
	///</summary>
	template <typename POSITION_TYPE>
	std::string SemanticTrajectory<POSITION_TYPE>::category_at(time_t time) const
	{
		int phase = timeslot->find_phase_of_time(time);
		return category_of_phase(phase);
	}

}
