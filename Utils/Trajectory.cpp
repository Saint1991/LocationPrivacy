#include "stdafx.h"
#include "Trajectory.h"

namespace Graph
{

	#pragma region TrajectoryState

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	TrajectoryState<POSITION_TYPE>::TrajectoryState(time_t time, std::shared_ptr<POSITION_TYPE> position) : time(time), position(position)
	{

	}


	///<summary>
	/// �o�̓f�[�^�̎擾
	///</summary>
	template <>
	std::unordered_map<std::string, std::string> TrajectoryState<Geography::LatLng>::get_export_data() const
	{
		std::unordered_map<std::string, std::string> ret = {
			{Geography::LatLng::LATITUDE, std::to_string(position->lat())},
			{Geography::LatLng::LONGITUDE, std::to_string(position->lng())},
			{TIME, std::to_string(time)}
		};
		return ret;
	}

	///<summary>
	/// �o�̓f�[�^�̎擾
	///</summary>
	template <>
	std::unordered_map<std::string, std::string> TrajectoryState<Graph::Coordinate>::get_export_data() const
	{
		std::unordered_map<std::string, std::string> ret = {
			{ Graph::Coordinate::X , std::to_string(position->x()) },
			{ Graph::Coordinate::Y, std::to_string(position->y()) },
			{ TIME, std::to_string(time) }
		};
		return ret;
	}
	
	#pragma endregion

	///<summary>
	/// �R���X�g���N�^
	/// times��UnixTimeStamp�̌n��
	///</summary>
	template <typename POSITION_TYPE>
	Trajectory<POSITION_TYPE>::Trajectory(std::unique_ptr<std::vector<time_t>> times, bool use_relative_time)
		: timeslot(std::make_shared<Time::TimeSlotManager>(std::move(times), use_relative_time))
	{
		node_ids = std::make_shared<std::vector<MapNodeIndicator>>(timeslot->phase_count(), MapNodeIndicator(INVALID, NodeType::INVALID));
		positions = std::make_shared<std::vector<std::shared_ptr<POSITION_TYPE>>>(timeslot->phase_count(), nullptr);
	}


	///<summary>
	/// �R���X�g���N�^
	/// times��TimeStamp������̌n��
	///</summary>
	template <typename POSITION_TYPE>
	Trajectory<POSITION_TYPE>::Trajectory(std::unique_ptr<std::vector<std::string>> times, bool use_relative_time)
		: timeslot(std::make_shared<Time::TimeSlotManager>(std::move(times), use_relative_time))
	{
		node_ids = std::make_shared<std::vector<MapNodeIndicator>>(timeslot->phase_count(), MapNodeIndicator(INVALID, NodeType::INVALID));
		positions = std::make_shared<std::vector<std::shared_ptr<POSITION_TYPE>>>(timeslot->phase_count(), nullptr);
	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	Trajectory<POSITION_TYPE>::Trajectory(std::shared_ptr<Time::TimeSlotManager> timeslot) 
		: timeslot(timeslot), positions(std::make_shared<std::vector<std::shared_ptr<POSITION_TYPE>>>(timeslot->phase_count(), nullptr)), 
		node_ids(std::make_shared<std::vector<MapNodeIndicator>>(timeslot->phase_count(), MapNodeIndicator(INVALID, NodeType::INVALID)))
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	Trajectory<POSITION_TYPE>::Trajectory(std::unique_ptr<std::vector<std::string>> times, std::shared_ptr<std::vector<Graph::MapNodeIndicator>> node_ids, std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE>>> positions, bool use_relative_time = true)
		: timeslot(std::make_shared<Time::TimeSlotManager>(std::move(times))), node_ids(node_ids), positions(positions)
	{

	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	Trajectory<POSITION_TYPE>::~Trajectory()
	{
	}

	template <typename POSITION_TYPE>
	size_t Trajectory<POSITION_TYPE>::phase_count() const
	{
		return timeslot->phase_count();
	}

	///<summary>
	/// �w�肵��Phase�ɂ�����ʒu��ݒ肷��
	///</summary>
	template <typename POSITION_TYPE>
	bool Trajectory<POSITION_TYPE>::set_position_of_phase(int phase, const POSITION_TYPE& position)
	{
		if (phase < 0 || positions->size() <= phase) return false;
		positions->at(phase) = std::make_shared<POSITION_TYPE>(position);
		return true;
	}


	///<summary>
	/// �w�肵���o�ߎ����ɂ�����ʒu��ݒ肷��
	///</summary>
	template <typename POSITION_TYPE>
	bool Trajectory<POSITION_TYPE>::set_position_at(time_t time, const POSITION_TYPE& position)
	{
		int phase = timeslot->find_phase_of_time(time);
		return set_position_of_phase(phase, position);
	}


	///<summary>
	/// �w�肵��Phase�ɂ�����ʒu���擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<POSITION_TYPE const> Trajectory<POSITION_TYPE>::position_of_phase(int phase) const
	{
		if (phase < 0 || positions->size() <= phase) return nullptr;
		return positions->at(phase);
	}

	///<summary>
	/// �w�肵���o�ߎ����ɂ�����ʒu���擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<POSITION_TYPE const> Trajectory<POSITION_TYPE>::position_at(time_t time) const
	{
		int phase = timeslot->find_phase_of_time(time);
		return position_of_phase(phase);
	}


	///<summary>
	/// ���n�񏇂ɂ��ׂĂ�Phase, �����C�ʒu�ɂ���execute_function�����s����
	///</summary>
	template <typename POSITION_TYPE>
	void Trajectory<POSITION_TYPE>::foreach(const std::function<void(int, time_t, std::shared_ptr<POSITION_TYPE const>)>& execute_function) const
	{
		timeslot->for_each_time([this, &execute_function](time_t time, long interval, int phase) {
			std::shared_ptr<POSITION_TYPE const> position = positions->at(phase);
			execute_function(phase, time, position);
		});
	}


	///<summary>
	/// �t�@�C���G�N�X�|�[�g�p�g���W�F�N�g���f�[�^���擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::list<std::shared_ptr<IO::FileExportable const>> Trajectory<POSITION_TYPE>::get_export_data() const
	{
		std::list<std::shared_ptr<IO::FileExportable const>> ret;
		timeslot->for_each_time([this, &ret](time_t time, long interval, int phase) {
			std::shared_ptr<POSITION_TYPE> position = positions->at(phase);
			std::shared_ptr<IO::FileExportable const> data = std::make_shared<TrajectoryState<POSITION_TYPE> const>(time, position);
			ret.push_back(data);
		});
		return ret;
	}
}

