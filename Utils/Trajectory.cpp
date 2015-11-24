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
	///</summary>
	template <typename POSITION_TYPE>
	Trajectory<POSITION_TYPE>::Trajectory(std::shared_ptr<Time::TimeSlotManager const> timeslot) 
		: timeslot(timeslot), positions(std::make_shared<std::vector<std::shared_ptr<POSITION_TYPE>>>(timeslot->phase_count(), nullptr)), 
		visited_node_ids(std::make_shared<std::vector<MapNodeIndicator>>(timeslot->phase_count(), MapNodeIndicator(INVALID, NodeType::INVALID)))
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	Trajectory<POSITION_TYPE>::Trajectory(std::shared_ptr<Time::TimeSlotManager const> timeslot, std::shared_ptr<std::vector<Graph::MapNodeIndicator>> node_ids, std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE>>> positions)
		: timeslot(timeslot), visited_node_ids(node_ids), positions(positions)
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
	bool Trajectory<POSITION_TYPE>::set_position_of_phase(int phase, const MapNodeIndicator& node_id, const POSITION_TYPE& position)
	{
		if (phase < 0 || positions->size() <= phase) return false;
		visited_node_ids->at(phase) = node_id;
		positions->at(phase) = std::make_shared<POSITION_TYPE>(position);
		return true;
	}


	///<summary>
	/// �w�肵���o�ߎ����ɂ�����ʒu��ݒ肷��
	///</summary>
	template <typename POSITION_TYPE>
	bool Trajectory<POSITION_TYPE>::set_position_at(time_t time, const MapNodeIndicator& node_id, const POSITION_TYPE& position)
	{
		int phase = timeslot->find_phase_of_time(time);
		return set_position_of_phase(phase, node_id, position);
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
	/// �w�肵��Phase�ɂ�����ʒu�ƃm�[�h��ID���擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::pair<MapNodeIndicator, std::shared_ptr<POSITION_TYPE const>> Trajectory<POSITION_TYPE>::read_node_pos_info_of_phase(int phase) const
	{
		MapNodeIndicator node_id = visited_node_ids->at(phase);
		std::shared_ptr<POSITION_TYPE const> pos = position_of_phase(phase);
		return std::make_pair(node_id, pos);
	}

	///<summary>
	/// �w�肵���o�ߎ����ɂ�����ʒu�ƃm�[�h��ID���擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::pair<MapNodeIndicator, std::shared_ptr<POSITION_TYPE const>> Trajectory<POSITION_TYPE>::read_node_pos_info_at(time_t time) const
	{
		int phase = timeslot->find_phase_of_time(time);
		return read_node_pos_info_of_phase(phase);
	}


	///<summary>
	/// �^�C���X���b�g��ǂݐ�p�Ŏ擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<Time::TimeSlotManager const> Trajectory<POSITION_TYPE>::read_timeslot() const
	{
		return timeslot;
	}

	///<summary>
	/// �w�肵�������ɑΉ�����phase���擾����
	///</summary>
	template <typename POSITION_TYPE>
	int Trajectory<POSITION_TYPE>::find_phase_of_time(time_t time) const
	{
		return timeslot->find_phase_of_time(time);
	}

	///<summary>
	/// �g���W�F�N�g���S�̂��܂���ŏ��̒����`�̈���擾����D
	///</summary>
	template <typename POSITION_TYPE>
	Rectangle<Geography::LatLng> Trajectory<POSITION_TYPE>::get_trajectory_boundary() const
	{
		double min_lng = 180.0;
		double min_lat = 90.0;
		double max_lng = -180.0;
		double max_lat = -90.0;

		for (std::vector<std::shared_ptr<POSITION_TYPE>>::const_iterator iter = positions->begin(); iter != positions->end(); iter++) {
			double lat = (*iter)->y();
			double lng = (*iter)->x();
			min_lng = min(min_lng, lng);
			min_lat = min(min_lat, lat);
			max_lng = max(max_lng, lng);
			max_lat = max(max_lat, lat);
		}

		return Rectangle<Geography::LatLng>(max_lat, min_lng, min_lat, max_lng);
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

