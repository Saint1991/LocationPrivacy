#include "stdafx.h"
#include "Trajectory.h"

namespace Graph
{

	#pragma region TrajectoryState

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	TrajectoryState<POSITION_TYPE>::TrajectoryState(time_t time, std::shared_ptr<POSITION_TYPE const> position, const std::string& venue_name) : time(time), position(position), venue_name(venue_name)
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
			{TIME, std::to_string(time)},
			{VENUE_NAME, venue_name}
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
			{ TIME, std::to_string(time) },
			{ VENUE_NAME, venue_name}
		};
		return ret;
	}
	
	#pragma endregion

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	Trajectory<POSITION_TYPE>::Trajectory(std::shared_ptr<Time::TimeSlotManager const> timeslot) 
		: timeslot(timeslot), positions(std::make_shared<std::vector<std::shared_ptr<POSITION_TYPE const>>>(timeslot->phase_count(), nullptr)), 
		visited_node_ids(std::make_shared<std::vector<MapNodeIndicator>>(timeslot->phase_count(), MapNodeIndicator(INVALID, NodeType::INVALID))),
		venue_names(std::make_shared<std::vector<std::string>>(timeslot->phase_count(), ""))
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE>
	Trajectory<POSITION_TYPE>::Trajectory(std::shared_ptr<Time::TimeSlotManager const> timeslot, std::shared_ptr<std::vector<Graph::MapNodeIndicator>> node_ids, std::shared_ptr<std::vector<std::shared_ptr<POSITION_TYPE const>>> positions, std::shared_ptr<std::vector<std::string>> venue_names)
		: timeslot(timeslot), visited_node_ids(node_ids), positions(positions), venue_names(venue_names)
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
	bool Trajectory<POSITION_TYPE>::set_position_of_phase(int phase, const MapNodeIndicator& node_id, const POSITION_TYPE& position, const std::string& venue_name)
	{
		if (phase < 0 || positions->size() <= phase) return false;
		visited_node_ids->at(phase) = node_id;
		positions->at(phase) = std::make_shared<POSITION_TYPE>(position);
		venue_names->at(phase) = venue_name;
		return true;
	}


	///<summary>
	/// �w�肵���o�ߎ����ɂ�����ʒu��ݒ肷��
	///</summary>
	template <typename POSITION_TYPE>
	bool Trajectory<POSITION_TYPE>::set_position_at(time_t time, const MapNodeIndicator& node_id, const POSITION_TYPE& position, const std::string& venue_name)
	{
		int phase = timeslot->find_phase_of_time(time);
		return set_position_of_phase(phase, node_id, position, venue_name);
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
	/// �w�肵��Phase�ɂ�����State���擾����
	///</summary>
	template <typename POSITION_TYPE>
	std::shared_ptr<TrajectoryState<POSITION_TYPE> const> Trajectory<POSITION_TYPE>::read_state_of_phase(int phase) const
	{
		time_t time = timeslot->time_of_phase(phase);
		std::shared_ptr<POSITION_TYPE const> position = positions->at(phase);
		std::string venue_name = venue_names->at(phase);
		return std::make_shared<TrajectoryState<POSITION_TYPE>>(time, position, venue_name);
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

		for (std::vector<std::shared_ptr<POSITION_TYPE const>>::const_iterator iter = positions->begin(); iter != positions->end(); iter++) {
			double lat = (*iter)->y();
			double lng = (*iter)->x();
			min_lng = min(min_lng, lng);
			min_lat = min(min_lat, lat);
			max_lng = max(max_lng, lng);
			max_lat = max(max_lat, lat);
		}

		return Rectangle<Geography::LatLng>(max_lat + 0.01, min_lng - 0.01, min_lat - 0.01, max_lng + 0.01);
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
	/// ���n�񏇂ɊePhase, �OPhase����̌o�ߎ���, State�ɂ���execute_function�����s����
	///</summary>
	template <typename POSITION_TYPE>
	void Trajectory<POSITION_TYPE>::foreach_state(const std::function<void(int, long, const Graph::MapNodeIndicator&, std::shared_ptr<TrajectoryState<POSITION_TYPE> const>)>& execute_function) const
	{
		timeslot->for_each_time([this, &execute_function](time_t time, long interval, int phase) {
			std::shared_ptr<POSITION_TYPE const> position = positions->at(phase);
			std::string venue_name = venue_names->at(phase);
			Graph::MapNodeIndicator visited_node = visited_node_ids->at(phase);
			execute_function(phase, interval, visited_node, std::make_shared<TrajectoryState<POSITION_TYPE> const>(time, position, venue_name));
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
			std::shared_ptr<POSITION_TYPE const> position = positions->at(phase);
			std::string venue_name = venue_names->at(phase);
			std::shared_ptr<IO::FileExportable const> data = std::make_shared<TrajectoryState<POSITION_TYPE> const>(time, position, venue_name);
			ret.push_back(data);
		});
		return ret;
	}
}

