#include "stdafx.h"
#include "PauseMobileEntity.h"


namespace Entity
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	VisitedPoiInfo::VisitedPoiInfo()
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	VisitedPoiInfo::~VisitedPoiInfo()
	{
	}

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::PauseMobileEntity(entity_id id, std::shared_ptr<Time::TimeSlotManager const> timeslot)
		: MobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>(id, timeslot),
		now_pause_time_list(std::vector<double>(timeslot->phase_count(), 0)),
		now_speed_list(std::vector<double>(timeslot->phase_count(), 0)),
		visited_pois_info_list_id(0)
	{
	}

	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::~PauseMobileEntity()
	{
	}

	///<summary>
	/// �K��POI����k�Ǘ�����ID���擾����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_visited_pois_info_list_id()
	{
		return visited_pois_info_list_id;
	}

	///<summary>
	/// �K��POI�����Ǘ�����ID�����Z�b�g����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::clear_visited_pois_info_list_id()
	{
		visited_pois_info_list_id = 0;
	}


	///<summary>
	/// �K��POI�����Z�b�g����D�K��n�_�̓o�^�̎��͂�������g��
	/// arrive_phase���L�^.�������Cpause_phase�͓o�^���Ȃ��Dpause_phase��s��~���ԓo�^�̍ۂɍs���D
	/// visited_poi_info��push_back����O�ɁCpause_phases�̓N���A���邱��
	/// push_back��́Carrive_phase��p���āC�\�[�g���s���D
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_visited_poi_of_phase(int phase, const Graph::MapNodeIndicator& node_id, const Geography::LatLng& position)
	{
 		trajectory->set_position_of_phase(phase, node_id, position);

		//�����C�J�n�_�ȊO�̑}���̎��́Cpause_phases���N���A����D
		if (!visited_pois_info_list.empty()) {
			clear_visited_poi_info();
		}

		visited_poi_info.visited_poi = std::make_pair(node_id, position);
		visited_poi_info.arrive_phase = phase;
		visited_pois_info_list.push_back(visited_poi_info);

		//sort��Carrive_phase���Ƀ\�[�g���Ă����D
		sort_pois_order_by_arrive_phase();
	}

	///<summary>
	/// ���L�n�_�ݒ莞�̖K��POI�����Z�b�g����
	/// MobileEntity���I�[�o�[���C�h�����֐�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_crossing_position_of_phase(int phase, const Graph::MapNodeIndicator& node_id, Geography::LatLng position, const std::string& venue_name)
	{
		register_as_cross_position(phase);
		set_visited_poi_of_phase(phase, node_id, position);
	}

	///<summary>
	/// ���ݒ�~��or���݌������Ă���POI���擾
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::pair<Graph::MapNodeIndicator, POSITION_TYPE> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_poi()
	{
		return visited_pois_info_list.at(visited_pois_info_list_id).visited_poi;
	}

	///<summary>
	/// i�ԖڂɖK��\���POI���擾
	/// ����i�Ԗڂ̗\��K��POI���Ȃ������ꍇ�́C�ŏI�t�F�[�Y��position��Ԃ�l�Ƃ���D
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::pair<Graph::MapNodeIndicator, POSITION_TYPE> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_any_poi(int i)
	{
		int last_phase = trajectory->phase_count() - 1;
		return i <= get_visited_pois_num() 
			? visited_pois_info_list.at(i).visited_poi : std::make_pair(trajectory->read_node_pos_info_of_phase(last_phase).first, *trajectory->read_node_pos_info_of_phase(last_phase).second);
	}

	///<summary>
	/// ���Ɍ�����POI�̏����擾
	/// �������̗̂\��K��POI���Ȃ������ꍇ�́C�ŏI�t�F�[�Y��position��Ԃ�l�Ƃ���D
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::pair<Graph::MapNodeIndicator, POSITION_TYPE> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_next_poi()
	{
		int last_phase = trajectory->phase_count() - 1;
		return (visited_pois_info_list_id + 1) <= get_visited_pois_num()
			? visited_pois_info_list.at(visited_pois_info_list_id + 1).visited_poi : std::make_pair(trajectory->read_node_pos_info_of_phase(last_phase).first, *trajectory->read_node_pos_info_of_phase(last_phase).second);
	}
	

	///<summary>
	/// visited_poi_info�̏����N���A����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::clear_visited_poi_info()
	{
		while (!visited_poi_info.pause_phases.empty()) {
			visited_poi_info.pause_phases.pop_back();
		}
		visited_poi_info.arrive_phase = 0;
		visited_poi_info.dest_rest_time = 0.0;
		visited_poi_info.pause_time = 0.0;
		visited_poi_info.rest_pause_time_when_departing = 0.0;
		visited_poi_info.starting_speed = 0.0;

	}

	///<summary>
	/// visited_pois_info_list��arrive_phase�̏��Ƀ\�[�g����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::sort_pois_order_by_arrive_phase()
	{
		std::sort(visited_pois_info_list.begin(), visited_pois_info_list.end(),
			[](VisitedPoiInfo& poi1, VisitedPoiInfo& poi2) {
			return poi1.arrive_phase < poi2.arrive_phase;
		});
	}

	///<summary>
	/// �\��K��POI�̐������߂�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_visited_pois_num()
	{
		return visited_pois_info_list.size();
	}

	///<summary>
	/// ���ɖK��\��̖K��POI�̒�~�t�F�[�Y�����߂�D
	/// ��~���̏ꍇ�́C����phase��Ԃ��C�ړ����̏ꍇ�́C�������Ă���K��n�_�̗\�肵�Ă��铞�����Ԃɒl����phase��Ԃ�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::vector<int> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_pause_phases()
	{
		return visited_pois_info_list.at(visited_pois_info_list_id).pause_phases;
	}

	///<summary>
	/// �S�Ă̖K��\��POI�̒�~phase�����߂�D
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::vector<int> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_all_pause_phases()
	{
		std::vector<int> all_pause_phases;

		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			for (std::vector<int>::iterator iter2 = iter->pause_phases.begin(); iter2 != iter->pause_phases.end(); iter2++) {
				all_pause_phases.push_back(*iter2);
			}
		}

		return all_pause_phases;
	}

	///<summary>
	/// ���ɖK��\��̒�~POI�̓�������phase��set����D
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_pause_phases(int pause_phase)
	{
		visited_poi_info.pause_phases.push_back(pause_phase);
		visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.push_back(pause_phase);
	}

	///<summary>
	/// ���ɖK��\��̒�~POI�̓�������phase��set����D
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_pause_phases_using_arrive_phase(int arrive_phase, int pause_phase) {
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				iter->pause_phases.push_back(pause_phase);
				visited_poi_info.pause_phases.push_back(pause_phase);
				return;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// ���ɖK��\��̒�~POI�̓�������phase�����߂�D
	/// ��~���̏ꍇ�́C����phase��Ԃ��C�ړ����̏ꍇ�́C�������Ă���K��n�_�̗\�肵�Ă��铞�����Ԃɒl����phase��Ԃ�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_arrive_phase()
	{
		return visited_pois_info_list.at(visited_pois_info_list_id).arrive_phase;
	}

	///<summary>
	/// ���ɖK��\��̒�~POI�̓�������phase��pause_phase�𗘗p���ċ��߂�D
	/// ��~���̏ꍇ�́C����phase��Ԃ��C�ړ����̏ꍇ�́C�������Ă���K��n�_�̗\�肵�Ă��铞�����Ԃɒl����phase��Ԃ�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_arrive_phase_using_pause_phase(int pause_phase)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			for (std::vector<int>::iterator iter2 = iter->pause_phases.begin(); iter2 != iter->pause_phases.end(); iter2++) {
				if (*iter2 == pause_phase) return iter->arrive_phase;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// i�ԖڂɖK��\��̒�~POI�̓�������phase�����߂�D
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_any_arrive_phase(int i)
	{
		return visited_pois_info_list.at(i).arrive_phase;
	}


	///<summary>
	/// �K��POI�̒�~���Ԃ�ύX�s�\�ȏ�Ԃŋ��߂�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_pause_time() const
	{
		return visited_pois_info_list.at(visited_pois_info_list_id).pause_time;
	}

	///<summary>
	/// �K��POI�̒�~���Ԃ�ύX�\�ȏ�Ԃŋ��߂�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_pause_time()
	{
		return visited_pois_info_list.at(visited_pois_info_list_id).pause_time;
	}


	///<summary>
	/// �K��POI�̒�~���Ԃ�ύX�\�ȏ�Ԃŋ��߂�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_pause_time_using_arrive_phase(int arrive_phase)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				return iter->pause_time;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// i�ԖڂɖK��\��̒�~���Ԃ�ύX�\�ȏ�Ԃŋ��߂�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_any_poi_pause_time(int i)
	{
		return i <= get_visited_pois_num() ? visited_pois_info_list.at(i).pause_time : 0;
	}

	///<summary>
	/// �K��POI���ɁCint�^�̒�~���ԂƓ���phase��set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_pause_time(int phase, int pause_time)
	{
		visited_poi_info.pause_phases.push_back(phase);
		visited_poi_info.pause_time = pause_time;

		visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.push_back(phase);
		visited_pois_info_list.at(visited_pois_info_list_id).pause_time = pause_time;
	}

	///<summary>
	/// �K��POI���ɁCdouble�^�̒�~���ԂƓ���phase��set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_pause_time(int phase, double pause_time)
	{
		visited_poi_info.pause_phases.push_back(phase);
		visited_poi_info.pause_time = pause_time;

		visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.push_back(phase);
		visited_pois_info_list.at(visited_pois_info_list_id).pause_time = pause_time;
	}

	///<summary>
	/// �K��POI���ɁCdouble�^�̒�~���ԂƓ���phase��set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_pause_time_using_arrive_phase(int arrive_phase, double pause_time)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				visited_poi_info.pause_phases.push_back(arrive_phase);
				visited_poi_info.pause_time = pause_time;

				iter->pause_phases.push_back(arrive_phase);
				iter->pause_time = pause_time;
				return;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// �K��POI����,int�^�̃����_���Ȓ�~���ԂƓ���phase��set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_pause_time(int phase, int min, int max)
	{
		Math::Probability generator;
		int pause_time = generator.uniform_distribution(min, max);

		visited_poi_info.pause_phases.push_back(phase);
		visited_poi_info.pause_time = pause_time;

		visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.push_back(phase);
		visited_pois_info_list.at(visited_pois_info_list_id).pause_time = pause_time;
	}

	///<summary>
	/// �����_���Ȓl��double�^�Œ�~���ԂƂ���set����
	/// �ŏ���pause_phase��pause_phases��push_back���邱�Ƃɒ���
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_pause_time(int phase, double min, double max)
	{
		Math::Probability generator;
		double pause_time = generator.uniform_distribution(min, max);

		visited_poi_info.pause_phases.push_back(phase);
		visited_poi_info.pause_time = pause_time;

		visited_pois_info_list.at(visited_pois_info_list_id).pause_phases.push_back(phase);
		visited_pois_info_list.at(visited_pois_info_list_id).pause_time = pause_time;
	}

	///<summary>
	/// �����_���Ȓl��double�^�Œ�~���ԂƂ���set����
	/// �ŏ���pause_phase��pause_phases��push_back���邱�Ƃɒ���
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_pause_time_using_arrive_phase(int arrive_phase, int phase, double min, double max) {

		Math::Probability generator;
		double pause_time = generator.uniform_distribution(min, max);

		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				visited_poi_info.pause_phases.push_back(phase);
				visited_poi_info.pause_time = pause_time;

				iter->pause_phases.push_back(phase);
				iter->pause_time = pause_time;
				return;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// �K��POI����speed��set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_starting_speed_at_poi(double speed)
	{
		visited_poi_info.starting_speed = speed;
		visited_pois_info_list.at(visited_pois_info_list_id).starting_speed = speed;
	}

	///<summary>
	/// �K��POI����speed��set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_starting_speed_at_poi_using_arrive_phase(int arrive_phase, double speed)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				iter->starting_speed = speed;
				visited_poi_info.starting_speed = speed;
				return;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// �K��POI����speed��randam��set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_starting_speed_at_poi(double average_speed, double speed_range)
	{
		Math::Probability generator;
		double min = average_speed - 0.5* speed_range;
		double max = average_speed + 0.5* speed_range;
		double entity_speed = generator.uniform_distribution(min, max);

		visited_poi_info.starting_speed = entity_speed;
		visited_pois_info_list.at(visited_pois_info_list_id).starting_speed = entity_speed;
	}

	///<summary>
	/// �K��POI����speed��randam��set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_starting_speed_at_poi_using_arrive_phase(int arrive_phase, double average_speed, double speed_range)
	{
		Math::Probability generator;
		double min = average_speed - 0.5* speed_range;
		double max = average_speed + 0.5* speed_range;
		double entity_speed = generator.uniform_distribution(min, max);
		
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				iter->starting_speed = entity_speed;
				return;
			}
		}
		throw std::invalid_argument("Not Found");
		
	}


	///<summary>
	/// POI���o�����̑��x��visited_pois_info_list_id��p���Ď擾����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_starting_speed()
	{
		return visited_pois_info_list.at(visited_pois_info_list_id).starting_speed;
	}

	///<summary>
	/// POI���o�����̑��x��arrive_phase��p���Ď擾����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_starting_speed_using_arrive_phase(int arrive_phase)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) return iter->starting_speed;
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// POI���o�����̑��x��arrive_phase��p���Ď擾����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_starting_speed_using_pause_phase(int pause_phase)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			for (std::vector<int>::iterator iter2 = iter->pause_phases.begin(); iter2 != iter->pause_phases.end(); iter2++) {
				if (*iter2 == pause_phase) return iter->starting_speed;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// POI�o�����̗]�莞�Ԃ�o�^
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_rest_pause_time_when_departing(double rest_pause_time)
	{
		visited_poi_info.rest_pause_time_when_departing = rest_pause_time;
		visited_pois_info_list.at(visited_pois_info_list_id).rest_pause_time_when_departing = rest_pause_time;
	}

	///<summary>
	/// POI�o�����̗]�莞�Ԃ�,arrive_phase��p���ēo�^
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_rest_pause_time_when_departing_using_arrive_phase(int arrive_phase, double rest_pause_time)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				visited_poi_info.rest_pause_time_when_departing = rest_pause_time;
				iter->rest_pause_time_when_departing = rest_pause_time;
				return;
			}
		}
		throw std::invalid_argument("Not Found");
	}

	///<summary>
	/// POI�o�����̗]�莞�Ԃ�,pause_phase��p���ēo�^
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_rest_pause_time_when_departing_using_pause_phase(int pause_phase)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			for (std::vector<int>::iterator iter2 = iter->pause_phases.begin(); iter2 != iter->pause_phases.end(); iter2++) {
				if (*iter2 == pause_phase) return iter->rest_pause_time_when_departing;
			}
		}
		throw std::invalid_argument("Not Found");
	}
	

	///<summary>
	/// POI�������̗]�莞�Ԃ�o�^
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_dest_rest_time(double dest_rest_time)
	{
		visited_poi_info.dest_rest_time = dest_rest_time;
		visited_pois_info_list.at(visited_pois_info_list_id).dest_rest_time = dest_rest_time;
	}


	///<summary>
	/// POI�������̗]�莞�Ԃ�arrive_phase��p���ēo�^
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_dest_rest_time_using_arrive_phase(int arrive_phase, double dest_rest_time)
	{
		for (std::vector<VisitedPoiInfo>::iterator iter = visited_pois_info_list.begin(); iter != visited_pois_info_list.end(); iter++) {
			if (iter->arrive_phase == arrive_phase) {
				visited_poi_info.dest_rest_time = dest_rest_time;
				iter->dest_rest_time = dest_rest_time;
				return;
			}
		}
		throw std::invalid_argument("Not Found");
	}


	///<summary>
	/// ����phase�̎c���~���Ԃ��C�ύX�s�\�ȏ�Ԃŋ��߂�D
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_now_pause_time(int now_phase) const
	{
		return now_pause_time_list.at(now_phase);
	}

	///<summary>
	/// ����phase�̎c���~���Ԃ��C�ύX�\�ȏ�Ԃŋ��߂�D
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_now_pause_time(int now_phase)
	{
		return now_pause_time_list.at(now_phase);
	}

	///<summary>
	/// ����phase�̎c���~���Ԃ��Cset����D
	/// phase�ɂ�push_back���Ȃ��̂Œ���
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_now_pause_time(int now_phase, double time)
	{
		now_pause_time_list.at(now_phase) = time;
	}




	///<summary>
	/// ���݂̈ړ����x�����߂�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	double PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_now_speed(int phase) const
	{
		return now_speed_list.at(phase);
	}

	///<summary>
	/// ����̒l�̈ړ����x�����ݑ��x��set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_now_speed(int phase, double speed)
	{
		now_speed_list.at(phase) = speed;
	}


	///<summary>
	/// �����_���Ȓl�̈ړ����x��set����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::set_random_now_speed(int phase, double average_speed, double speed_range)
	{
		Math::Probability generator;
		double min = average_speed - 0.5* speed_range;
		double max = average_speed + 0.5* speed_range;
		double entity_speed = generator.uniform_distribution(min, max);

		now_speed_list.at(phase) = entity_speed;

	}


	///<summary>
	/// ��~���Ă��邩�ǂ����m�F����
	/// ���ݑ��x��0�̏ꍇ��~�Ɣ��f
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	bool PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::check_pause_condition(int now_phase)
	{
		return get_now_speed(now_phase) == 0.0 ? true : false;
	}

	///<summary>
	/// visited_pois_info_list_id���C���N�������g����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	void PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::increment_visited_pois_info_list_id()
	{
		visited_pois_info_list_id++;
	}


	///<summary>
	/// �g���W�F�N�g���f�[�^��ύX�\�ȏ�ԂŎ擾����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::shared_ptr<TRAJECTORY_TYPE> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::get_trajectory()
	{
		return trajectory;
	}


	///<summary>
	/// �������ݒ肳��Ă��Ȃ�Phase��S�Ď擾����
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	std::vector<int> PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::find_cross_not_set_phases_of_poi() const
	{
		std::vector<int> ret;
		for (int phase = 0; phase < cross_flg->size(); phase++) {
			node_pos_info info = trajectory->read_node_pos_info_of_phase(phase);
			if (!cross_flg->at(phase) && info.first.type() == Graph::NodeType::POI) ret.push_back(phase);
		}
		return ret;
	}

	///<summary>
	/// �������ݒ肳��Ă��Ȃ�������������_���Ɏ擾����
	/// �ݒ肳��Ă��Ȃ�phase�����݂��Ȃ��ꍇ��INVALID��Ԃ�
	///</summary>
	template <typename POSITION_TYPE, typename TRAJECTORY_TYPE>
	int PauseMobileEntity<POSITION_TYPE, TRAJECTORY_TYPE>::randomly_pick_cross_not_set_phase_of_poi() const
	{
		Math::Probability generator;
		std::vector<int> not_set_phases = find_cross_not_set_phases_of_poi();
		if (not_set_phases.size() == 0) return INVALID;
		return not_set_phases.at(generator.uniform_distribution(0, not_set_phases.size() - 1));
	}

}
