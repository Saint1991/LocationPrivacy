#include "stdafx.h"
#include "KatoBachelorMethod.h"

namespace Method
{

	///<summary>
	/// �R���X�g���N�^
	/// �����Simulator�ō쐬�����e����͂ւ̎Q�Ƃ�n��
	///</summary>
	KatoBachelorMethod::KatoBachelorMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement, Geography::LatLng, Graph::Trajectory<Geography::LatLng>>(map, user, requirement, time_manager),
		grid_list(std::vector<Grid>((time_manager->phase_count()/requirement->interval_of_base_phase)+1)),
		creating_dummy(nullptr)
	{
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	KatoBachelorMethod::~KatoBachelorMethod()
	{
	}


	///<summary>
	/// T[s]���Ƃ̃O���b�h�̈���쐬
	/// grid_length�̓O���b�h�S�̂̒���
	///</summary>
	std::vector <Graph::Rectangle<Geography::LatLng>> KatoBachelorMethod::make_grid(double grid_area, const Geography::LatLng& center, int cell_num_on_side)
	{
		double grid_length = std::sqrt(grid_area);//grid�S�̂̈�ӂ̒����D�����̈�́�
		double cell_length = grid_length / cell_num_on_side;//�Z������̒����`�̒���
	
		//����(cell_length)���ܓx�o�x�̒P�ʂɕϊ�
		double length_translated_lat = Geography::GeoCalculation::calc_translated_point(center, cell_length, M_PI_2 * 3).lat() - center.lat();
		double length_translated_lng = Geography::GeoCalculation::calc_translated_point(center, cell_length, 0).lng() - center.lng();

		//center�𒆐S�Ƃ���grid�𐶐��������́C��ԍ���ɑ�������cell�̎l�_�̈ܓx�o�x
		double top = center.lat() + 1.5 * length_translated_lat;
		double left = center.lng() - 1.5 * length_translated_lng;
		double bottom = center.lat() + 0.5 * length_translated_lat;
		double right = center.lng() - 0.5 * length_translated_lng;

		std::vector<Graph::Rectangle<Geography::LatLng>> grid_list;//�O���b�h�S�̂��Ǘ����郊�X�g

		double base_left = left;//����̐����`��left�����[�v�̍ۂ̊�Ƃ���
		double base_right = right;//����̐����`��right�����[�v�̍ۂ̊�Ƃ���

		for (int i = 0; i < cell_num_on_side; i++)
		{
			for (int j = 0; j < cell_num_on_side; j++)
			{
				grid_list.push_back(Graph::Rectangle<Geography::LatLng>(top, left, bottom, right));
				right += length_translated_lng;
				left += length_translated_lng;
			}

			top -= length_translated_lat;
			bottom -= length_translated_lat;
			left = base_left;
			right = base_right;
		}

		return grid_list;
	}


	///<summary>
	/// �O���b�h�e�[�u����start����end�܂ł̃G���e�B�e�B�̍��v���擾
	///</summary>
	std::vector<int> KatoBachelorMethod::get_total_num_of_each_cell_at_interval_phase(std::vector<std::vector<int>>& entities_num_table, int start, int end) {
		std::vector<int> total_entity_num_interval_phase(CELL_NUM_ON_SIDE*CELL_NUM_ON_SIDE, 0);

		for (int cell_id = 0; cell_id < CELL_NUM_ON_SIDE*CELL_NUM_ON_SIDE; cell_id++)
		{
			int temp = 0;
			for (int interval_of_base_phase = start; interval_of_base_phase < end; interval_of_base_phase++)
			{
				temp += entities_num_table.at(cell_id).at(interval_of_base_phase);
			}
			total_entity_num_interval_phase.at(cell_id) = temp;
		}
		return total_entity_num_interval_phase;
	}

	///<summary>
	/// �O���b�h�e�[�u���̊e�O���b�h���Ƃ̑S�t�F�[�Y�̃G���e�B�e�B�̍��v���擾
	///</summary>
	std::vector<int> KatoBachelorMethod::get_total_num_of_each_cell_at_all_phase(std::vector<std::vector<int>>& entities_num_table) {
		std::vector<int> total_entity_num_all_phase = get_total_num_of_each_cell_at_interval_phase(entities_num_table, 0, entities_num_table.at(0).size());
		return total_entity_num_all_phase;
	}

	///<summary>
	/// start_phase����end_phase�܂ł̃G���e�B�e�B����������cell_id�������_���ɕ��בւ���vector�Ŏ擾���܂�
	///</summary>
	int KatoBachelorMethod::get_min_cell_id_of_entities_num(std::vector<int>& total_entity_num) const
	{
		int temp = 1000;
		int cell_id = 0;
		std::vector<int> cell_list_of_min_entities_num;
		for (std::vector<int>::const_iterator iter = total_entity_num.begin(); iter != total_entity_num.end(); iter++, cell_id++) {
			if (*iter <= temp)
			{
				temp = *iter;
				cell_list_of_min_entities_num.push_back(cell_id);
			}
		}
		std::random_device device;
		std::mt19937_64 generator(device());
		std::shuffle(cell_list_of_min_entities_num.begin(), cell_list_of_min_entities_num.end(), generator);
				
		return cell_list_of_min_entities_num.front();
	}


	///<summary>
	/// min_cell_id�̃Z���ōŏ��ɂȂ�phase�������_���Ŏ擾
	/// start����end�܂ł��w��
	///</summary>
	int KatoBachelorMethod::get_min_phase_of_min_cell_id(std::vector<std::vector<int>>& entities_num_table, int min_cell_id, int start, int end) const
	{
		int temp = 1000;
		int base_phase_id = start;
		std::vector<int> base_phase_list_of_min_entities_num;
		for (int i = start; i < end; i++, base_phase_id++) {
			if (entities_num_table.at(min_cell_id).at(i) <= temp) {
				temp = entities_num_table.at(min_cell_id).at(i);
				base_phase_list_of_min_entities_num.push_back(base_phase_id);
			}
		}

		std::random_device device;
		std::mt19937_64 generator(device());
		std::shuffle(base_phase_list_of_min_entities_num.begin(), base_phase_list_of_min_entities_num.end(), generator);

		return base_phase_list_of_min_entities_num.front();
	}


	///<summary>
	/// Rectangle�Ɋ܂܂��POI�̃��X�g���擾
	/// boundary����poi��������Ȃ��ꍇ�͔͈͂��L���čĒT��
	///</summary>
	std::vector<std::shared_ptr<Map::BasicPoi const>> KatoBachelorMethod::candidate_pois_list(Graph::Rectangle<Geography::LatLng>& boundary) {
		std::vector<std::shared_ptr<Map::BasicPoi const>> candidate_pois_list = map->find_pois_within_boundary(boundary);
		double length = 0.005;
		//�����͈͓���POI��������Ȃ�������C�͈͂��L���čČv�Z
		if (candidate_pois_list.size() == 0) {
			while (candidate_pois_list.size() == 0) {
				length += 0.001;
				boundary.top += 0.5 * length;
				boundary.left -= 0.5 * length;
				boundary.bottom -= 0.5 * length;
				boundary.right += 0.5 * length;
				candidate_pois_list = map->find_pois_within_boundary(boundary);
			}
		}

		std::random_device device;
		std::mt19937_64 generator(device());
		std::shuffle(candidate_pois_list.begin(), candidate_pois_list.end(), generator);

		return candidate_pois_list;
	}



	///<summary>
	/// position�Ԃ̃p�X����`��Ԃ���D
	/// source�ɂ͌���_,destination�ɂ͖ړI�n�Csource_phase�ɂ́C���肵�Ă���phase����͂��邱��.
	/// source�ɂ͊��ɒ�~�n�_�͓��͂���Ă���
	/// dest_rest_time���|�C���^�����ɂ��邱�ƂŁC�����̕Ԃ�l������.���̓������Ԃ̗]���Ԃ��D
	///</summary>
	void KatoBachelorMethod::linear_interpolation_of_path_between_positions(const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int *phase_id, int *dest_rest_time) {
		std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path(source, destination);

		//�O��̓������������������C�c��̒�~����
		int rest_pause_time = creating_dummy->get_pause_time(*phase_id) - *dest_rest_time;

		//�c��̒�~���Ԃ�phase�Ɋ��Z���Cphase�����߂�D
		div_t variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, requirement->service_interval);

		//��~���ԕ��C�ephase�ɒ�~�ꏊ�ƈړ����x(0)��o�^
		for (int i = 0; i <= variable_of_converted_pause_time_to_phase.quot; i++)
		{
			Geography::LatLng source_latlng = map->get_static_poi(source.id())->data->get_position();

			creating_dummy->set_position_of_phase(*phase_id, source, source_latlng);
			if (i != 0) creating_dummy->set_speed(*phase_id, 0);
			(*phase_id)++;
		}

		int init_flag = 1;//POI�����߂�ŏ��̃t�F�[�Y���ǂ����������t���O
		double pause_position_speed = creating_dummy->get_speed((*phase_id - 1) - variable_of_converted_pause_time_to_phase.quot);//��~�ʒu�̏o�����x��ێ����Ă���
		std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//path���������邽�߂�index

		//source����̋���
		//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
		//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
		double distance = variable_of_converted_pause_time_to_phase.rem * pause_position_speed;

		double distance_from_source_to_destination = map->shortest_distance(source, destination);
		
		Graph::MapNodeIndicator nearest_position = source;
		//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
		while (distance < distance_from_source_to_destination)
		{
			//�ŏ��͒�~���Ԃ�phase�Ɋ��Z�����Ƃ��̗]�蕪��distance�Ƃ��āC�ŒZ�H�̒��ň�ԋ߂�intersection��T���C���`��Ԃ���D
			while (distance > map->shortest_distance(source, *path_iter))
			{
				nearest_position = *path_iter;
				path_iter++;
			}

			double distance_between_nearest_intersection_and_arrive_position = distance - map->shortest_distance(source, nearest_position);
			Geography::LatLng nearest_latlng
				= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi(nearest_position.id())->data->get_position() : *map->get_static_node(nearest_position.id())->data;
			Geography::LatLng next_nearest_latlang
				= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi((*path_iter).id())->data->get_position() : *map->get_static_node((*path_iter).id())->data;
			double angle = Geography::GeoCalculation::lambert_azimuth_angle(nearest_latlng, next_nearest_latlang);

			Geography::LatLng arrive_position = Geography::GeoCalculation::calc_translated_point(nearest_latlng, distance_between_nearest_intersection_and_arrive_position, angle);


			creating_dummy->set_position_of_phase(*phase_id, Graph::MapNodeIndicator(Graph::NodeType::INVALID, Graph::NodeType::INVALID), arrive_position);
			(*phase_id)++;

			distance += requirement->service_interval * pause_position_speed;
		}

		//destination�̂Ƃ���܂ŕ⊮�ł�����Crest_time��ێ����Ă����I
		double distance_between_arrive_position_and_dest_position = distance - map->shortest_distance(source, *path_iter);
		*dest_rest_time
			= distance_between_arrive_position_and_dest_position == requirement->service_interval * pause_position_speed ?
			0 : distance_between_arrive_position_and_dest_position / pause_position_speed;
		//�ړI�n�̓o�^
		creating_dummy->set_position_of_phase(*phase_id, destination, map->get_static_poi(destination.id())->data->get_position());
	}




	///<summary>
	/// �������_�~�[(k�Ԗ�)�̊�n�_����ъ�n�_�������Ԃ̌���
	///</summary>
	void KatoBachelorMethod::decide_base_positions_and_arrive_time(int dummy_id)
	{
		int phase = 0;//phase
		const int GRID_TOTAL_NUM = CELL_NUM_ON_SIDE*CELL_NUM_ON_SIDE;//�O���b�h�̐�

		//�e�O���b�h�̊e�t�F�C�Y�ɂ�����entities�̐����L�����邽�߂�table(���I�z��)�̍쐬
		std::vector<std::vector<int>> entities_num_table(GRID_TOTAL_NUM, std::vector<int>((time_manager->phase_count()/requirement->interval_of_base_phase)+1, 0));

		//����phase�ɂ�����e�Z���ɑ��݂��郆�[�U�y�ѐ����ς݃_�~�[�̈ړ��o�H(��~�n�_)�̐�
		//�����Z����id�C�c��phase��\�����I�Q�����z��ŋL��
		//phase_interval�̊Ԋu�ŋL�^���Ă���
		//k-2�ڂ܂ł�table������Ă����āCk-1�ڂ��{�P���čX�V����΂��������ǂ�
		std::shared_ptr<Geography::LatLng const> center = entities->get_average_position_of_phase(phase);//���S�ʒu�����߂�
		Grid grid = make_grid(requirement->required_anonymous_area, *center, CELL_NUM_ON_SIDE);//phase���ƂɃO���b�h���쐬
		int grid_list_id = 0;
		int cell_id = 0;//�Z����id
		int end_phase = time_manager->phase_count();
		
		while (phase <= end_phase)
		{
			cell_id = 0;//�Z����id�̃��Z�b�g
			center = entities->get_average_position_of_phase(phase);//���S�ʒu�����߂�
			grid = make_grid(requirement->required_anonymous_area, *center, CELL_NUM_ON_SIDE);//phase���ƂɃO���b�h���쐬
			grid_list.at(grid_list_id) = grid;//����phase��Grid.grud_list_id�ŉ���ڂ���grid���������L�^����
						
			//����phase�̑S�ẴZ���́C�G���e�B�e�B�����v�Z(�\�̗���v�Z���邱�Ƃɑ���)
			for (std::vector<Graph::Rectangle<Geography::LatLng>>::iterator iter = grid_list.at(grid_list_id).begin(); iter != grid_list.at(grid_list_id).end(); iter++, cell_id++)
			{
				entities_num_table.at(cell_id).at(grid_list_id) = entities->get_entity_count_within_boundary(phase, *iter);
			}
			phase += requirement->interval_of_base_phase;//phase�̍X�V
			grid_list_id++;//grid_list_id�̃C���N�������g
		}

		//������phase�Őݒ肵�C���̎����x�[�X�œ����̈�m�ۂ̂��߂̒n�_���쐬
		//�Y����������̃t�F�[�Y�ɂ����āC�G���e�B�e�B���ŏ��ɂȂ�Z��id���擾
		//�e�Z���̑Sphase�̃G���e�B�e�B�̍��v(�\�̍s�̘a���v�Z���Ă��邱�Ƃɑ���)
		int start_of_cycle = 0;
		int end_of_cycle = requirement->cycle_of_interval_of_base_num;;
		std::vector<int> total_entity_num_at_interval_phase = get_total_num_of_each_cell_at_interval_phase(entities_num_table, start_of_cycle, end_of_cycle);
				
		//�I��������entities_table��size�ɂȂ�܂�!
		while (end_of_cycle <= entities_num_table.at(0).size()) {
			//start_phase����end_phase�܂łŁC�G���e�B�e�B�����ŏ��ƂȂ�Z��id�������_���ŋ��߂�D
			int min_cell_id = get_min_cell_id_of_entities_num(total_entity_num_at_interval_phase);
			
			//min_cell_id�̃Z���ōŏ��ɂ�base_phase��random�Ŏ擾
			//base_phase��interval_of_base_phase�̒��̐��Ȃ̂ŁC���ۂ�phase�͕�
			int base_phase = get_min_phase_of_min_cell_id(entities_num_table, min_cell_id, start_of_cycle, end_of_cycle);
			int real_phase = base_phase * requirement->interval_of_base_phase;
			//�擾����cell_id,phase�ɂ������~�n�_���擾
			//��l���z�Ń����_���擾
			//������Ȃ������ꍇ�̍L����傫���͍l�������ق����ǂ���������Ȃ�
			Graph::Rectangle<Geography::LatLng> cell = grid_list.at(base_phase).at(min_cell_id);
			std::vector<std::shared_ptr<Map::BasicPoi const>> poi_within_base_point_grid = candidate_pois_list(cell);
			std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_within_base_point_grid.begin();
			//��ԍŏ��̂ݓ��B�\�����l�������ɒ�~�n�_�����肷��D
			if (creating_dummy->find_previous_fixed_position(time_manager->phase_count()).first == INVALID) {
				creating_dummy->set_position_of_phase(base_phase, Graph::MapNodeIndicator((*poi)->get_id()), (*poi)->data->get_position());
				creating_dummy->set_random_speed(base_phase, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);
			}
			//��ӏ��ڈȍ~�̊�n�_�́C�ȑO�̊�n�_���瓞�B�\���𒲂ׂ��̂��Ɍ��肷��D
			else
			{
				std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_base_info = creating_dummy->find_previous_fixed_position(real_phase);
				int base_time_limit = time_manager->time_of_phase(real_phase) - time_manager->time_of_phase(previous_base_info.first) - requirement->min_pause_time;
				
				//���B�\�łȂ��ꍇ�͕ʂ�poi�ɂ���D
				//�����̑I�ѕ��͍l���Ȃ��Ƃ����Ȃ�����
				while (!map->is_reachable(previous_base_info.second.first, Graph::MapNodeIndicator((*poi)->get_id()), creating_dummy->get_speed(previous_base_info.first), base_time_limit)) {
					poi++;
				}
				
				//���������ŁC�Y������poi���Ȃ�������C���ɐ��̏��Ȃ��Z���̗̈���Q�Ƃ���Ƃ�����O������������K�v������D
				
				creating_dummy->set_position_of_phase(real_phase, Graph::MapNodeIndicator((*poi)->get_id()), (*poi)->data->get_position());
				creating_dummy->set_random_speed(real_phase, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);
			}

			start_of_cycle += requirement->cycle_of_interval_of_base_num;
			end_of_cycle += requirement->cycle_of_interval_of_base_num;
			total_entity_num_at_interval_phase = get_total_num_of_each_cell_at_interval_phase(entities_num_table, start_of_cycle, end_of_cycle);
		}
	}




	///<summary>
	/// �������_�~�[(k�Ԗ�)�̋��L�n�_����ы��L�n�_�������Ԃ̌���
	///</summary>
	void KatoBachelorMethod::decide_share_positions_and_arrive_time(int dummy_id)
	{
		//�����񐔂����Ȃ��G���e�B�e�B��D��I�Ɍ����Ώۂɂ��ČJ��Ԃ�
		std::list<std::pair<Entity::entity_id, int>> entity_list_order_by_cross = entities->get_entity_id_list_order_by_cross_count();
		for (std::list<std::pair<Entity::entity_id, int>>::const_iterator iter = entity_list_order_by_cross.begin(); iter != entity_list_order_by_cross.end(); iter++)
		{

			//�������̃_�~�[�̂��̂̓X�L�b�v
			if (iter->first > dummy_id) continue;

			//�����Ώۂ�ID(�����񐔍ŏ�)
			int cross_target = iter->first;
			int target_phase = INVALID;

			//BasicUser��Dummy���p�����Ă���̂Ń|�C���^����͉\
			//target���������郆�[�Uor�_�~�[
			//target_phases�͌������ݒ肳��Ă��Ȃ�phase�̈ꗗ
			std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> target = cross_target == 0 ? entities->get_user() : entities->get_dummy_by_id(cross_target);
			std::vector<int> target_phases = target->find_cross_not_set_phases();

			//�������ݒ�̎������烉���_���ɋ��L�n�_�ݒ�����݂�
			std::random_device device;
			std::mt19937_64 generator(device());
			std::shuffle(target_phases.begin(), target_phases.end(), generator);

			//�t�F�[�Y��0�łȂ����������ݒ�̃t�F�[�Y�Ɍ����n�_��ݒ肷��D
			for (std::vector<int>::const_iterator target_phase = target_phases.begin(); target_phase != target_phases.end(); target_phase++) {
				if (*target_phase == 0) continue;
				int share_phase = *target_phase;
				std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>> share_position = target->read_node_pos_info_of_phase(share_phase);

				//�������_�~�[�̊��ɒ�~�ʒu�����肵�Ă���t�F�[�Y�������L�t�F�[�Y���傫���ꍇ
				if (creating_dummy->find_previous_fixed_position(time_manager->phase_count()).first <= share_phase)
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_info = creating_dummy->find_previous_fixed_position(time_manager->phase_count());
					int previous_time_limit = time_manager->find_phase_of_time(share_phase) - time_manager->find_phase_of_time(previous_info.first) - creating_dummy->get_pause_time(previous_info.first);
					//���L�ꏊ�ɓ��B�\�Ȃ炻�̈ʒu��ݒ肵�C���B�s�\�Ȃ�΂�����x�ʂ̃t�F�[�Y������
					if (!map->is_reachable(previous_info.second.first, share_position.first, creating_dummy->get_speed(previous_info.first), previous_time_limit)) {
						goto ONCE_AGAIN;
					}
				}
				//�������_�~�[�̊��ɒ�~�ʒu�����肵�Ă���t�F�[�Y�������L�t�F�[�Y���������ꍇ
				//��������
				else if (creating_dummy->find_next_fixed_position(0).first >= share_phase)
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> next_info = creating_dummy->find_next_fixed_position(0);
					int next_time_limit = time_manager->find_phase_of_time(next_info.first) - time_manager->find_phase_of_time(share_phase) - creating_dummy->get_pause_time(share_phase);
					if (!map->is_reachable(share_position.first, next_info.second.first, target->get_speed(share_phase), next_time_limit)) {
						goto ONCE_AGAIN;
					}
				}
				//�������_�~�[�̊��ɒ�~�ʒu�����肵�Ă���t�F�[�Y�̊Ԃɂ���ꍇ
				else
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_info = creating_dummy->find_previous_fixed_position(share_phase);
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> next_info = creating_dummy->find_next_fixed_position(share_phase);
					//time_limit��previous�ł̒�~���Ԃ��l�����Ȃ���΂Ȃ�Ȃ����Ƃɒ���
					int previous_time_limit = time_manager->find_phase_of_time(share_phase) - time_manager->find_phase_of_time(previous_info.first) - creating_dummy->get_pause_time(previous_info.first);
					int next_time_limit = time_manager->find_phase_of_time(next_info.first) - time_manager->find_phase_of_time(share_phase) - creating_dummy->get_pause_time(share_phase);

					if (!map->is_reachable(previous_info.second.first, share_position.first, creating_dummy->get_speed(previous_info.first), previous_time_limit)
						&& !map->is_reachable(share_position.first, next_info.second.first, target->get_speed(share_phase), next_time_limit)) {
						goto ONCE_AGAIN;
					}
				}

				//Dmincross = += 1;
				//�������̃_�~�[�̌����� += 1;
				creating_dummy->set_crossing_position_of_phase(share_phase, share_position.first, *share_position.second);
				creating_dummy->set_speed(share_phase, target->get_speed(share_phase));
				int target_dummy_cross_count = target->get_cross_count();
				target_dummy_cross_count++;//���̂�����target�̌����񐔂̍��v�𐔂�����H
				break;

			ONCE_AGAIN:
				continue;
			}
			//���[�U�y�ѐ����ς݃_�~�[�̕��ό����񐔂����C�������_�~�[�̌����񐔂������Ȃ�܂ŋ��L�n�_��ݒ肷��
			if (creating_dummy->get_cross_count() > entities->get_all_entities_total_crossing_count() / dummy_id) break;
		}
	}



	///<summary>
	/// �������_�~�[(k�Ԗ�)�̈ړ��o�H�̌���
	///</summary>
	void KatoBachelorMethod::decide_destination_on_the_way(int dummy_id)
	{
		//�������_�~�[�̊��Ɍ��܂��Ă��钆�ōŏ��̒n�_���擾
		std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> dest_position = creating_dummy->find_next_fixed_position(0);

		//------------------------------------�������ʒu�̌��聫------------------------------------------------------//

		creating_dummy->set_pause_time(0, requirement->max_pause_time);//����phase�̒�~����
		creating_dummy->set_random_speed(0, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);//����phase��speed�̌���

		//�������_�~�[�̃v�����̒��ŁC��ԍŏ��̏ꏊ����0�b�܂ł͈̔�(�ő��~���Ԃ��l��)�œ����ł���POI���擾
		//��U���X�g�Ŏ擾���Ă���C���̒����烉���_���őI��
		double distance = creating_dummy->get_speed(0) * (time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(0));
		Geography::LatLng candidate_init_latlng = Geography::GeoCalculation::calc_translated_point(*dest_position.second.second, distance, M_PI * 1.5);
		//POI��T�����钷���`���擾�D�ړI�n�ɋ߂Â��������l��
		double top = dest_position.second.second->lat() - candidate_init_latlng.lat() > 0 ? dest_position.second.second->lat() : candidate_init_latlng.lat();
		double left = dest_position.second.second->lng() - candidate_init_latlng.lng() > 0 ? candidate_init_latlng.lng() : dest_position.second.second->lng();
		double bottom = dest_position.second.second->lat() - candidate_init_latlng.lat() < 0 ? dest_position.second.second->lat() : candidate_init_latlng.lat();
		double right = dest_position.second.second->lng() - candidate_init_latlng.lng() < 0 ? candidate_init_latlng.lng() : dest_position.second.second->lng();
		Graph::Rectangle<Geography::LatLng> rect1(top, left, bottom, right);

		std::vector<std::shared_ptr<Map::BasicPoi const>> init_pois_list = candidate_pois_list(rect1);
		std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator init_poi = init_pois_list.begin();
		int init_time_limit = time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(0) - creating_dummy->get_pause_time(0);

		while (!map->is_reachable(Graph::MapNodeIndicator((*init_poi)->get_id()), dest_position.second.first, creating_dummy->get_speed(0), init_time_limit)) {
			init_poi++;
		}

		//PPout��<position, start, pause��init>��ǉ�;
		creating_dummy->set_position_of_phase(0, Graph::MapNodeIndicator((*init_poi)->get_id()), (*init_poi)->data->get_position());

		//------------------------------------�������ʒu�̌��聪------------------------------------------------------//

		//�����ʒu�ȍ~������ɒ�~�ʒu����ς݂�phase�܂ł̒�~�n�_�̌���
		int phase_id = 1;
		int dest_rest_time = 0;
		while (phase_id < creating_dummy->find_previous_fixed_position(time_manager->phase_count()).first)
		{
			std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> decided_position = creating_dummy->find_previous_fixed_position(dest_position.first);
			int time_between_already_and_dest_position = time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(decided_position.first);
			//��_�Ԃ����ɋ߂��C���C�A���Œ�~�n�_�����܂��Ă��Ȃ��ꍇ�r����~�ʒu��ݒ�
			//already��dest�̓������Ԃ̍�>decided��dest�̈ړ����ԁ{MaxPauseTime
			while (time_between_already_and_dest_position > map->calc_necessary_time(decided_position.second.first, dest_position.second.first, creating_dummy->get_speed(decided_position.first)) + requirement->max_pause_time) {
				//position(phase_id)����position(dest)�Ԗڂ֓��B�\��POI����ЂƂ����_���Ŏ擾
				//already��dest�Ŏl�p�`�����C���̒��ɑ��݂���POI��I�����邱�Ƃŋ߂Â��������l��
				double top
					= decided_position.second.second->lat() >= dest_position.second.second->lat() ? decided_position.second.second->lat() : dest_position.second.second->lat();
				double left
					= decided_position.second.second->lng() <= dest_position.second.second->lng() ? decided_position.second.second->lng() : dest_position.second.second->lng();
				double bottom
					= decided_position.second.second->lat() <= dest_position.second.second->lat() ? decided_position.second.second->lat() : dest_position.second.second->lat();
				double right
					= decided_position.second.second->lng() >= dest_position.second.second->lng() ? decided_position.second.second->lng() : dest_position.second.second->lng();
				Graph::Rectangle<Geography::LatLng> rect2(top, left, bottom, right);

				std::vector<std::shared_ptr<Map::BasicPoi const>> on_the_way_pois_list = candidate_pois_list(rect2);
				std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi_on_the_way = on_the_way_pois_list.begin();

				//�r���ړI�n�̑��x��ݒ肷��
				//PauseTime���܂����܂�Ȃ��̂ŁC�Z�b�g�͂��Ȃ��D
				Math::Probability generator;
				double on_the_way_speed = generator.uniform_distribution(requirement->average_speed_of_dummy - 0.5 * requirement->speed_range_of_dummy, requirement->average_speed_of_dummy + 0.5 * requirement->speed_range_of_dummy);

				int total_pause_time_at_decided_and_dest_position
					= time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(decided_position.first)
					- map->calc_necessary_time(decided_position.second.first, (*poi_on_the_way)->get_id(), creating_dummy->get_speed(decided_position.first))
					- map->calc_necessary_time((*poi_on_the_way)->get_id(), dest_position.second.first, on_the_way_speed);

				double rate_of_spilit_pause_time = generator.uniform_distribution(0.35, 0.65);

				//����ς݂̓_�̒�~���Ԃ�ݒ�
				creating_dummy->set_pause_time(decided_position.first, total_pause_time_at_decided_and_dest_position * rate_of_spilit_pause_time);

				//�r���ړI�n�܂ł�path�����߂�
				linear_interpolation_of_path_between_positions(decided_position.second.first, (*poi_on_the_way)->get_id(), &phase_id, &dest_rest_time);

				//�r���ړI�n�̑��x�̃Z�b�g
				creating_dummy->set_speed(phase_id, on_the_way_speed);

				//�r���ړI�n�̒�~���Ԃ̃Z�b�g
				creating_dummy->set_pause_time(phase_id, total_pause_time_at_decided_and_dest_position * (1 - rate_of_spilit_pause_time));

				//decided_position�̍X�V
				decided_position = creating_dummy->find_previous_fixed_position(dest_position.first);
			}
			//decided_position�̒�~���Ԃ����߂�
			//��~���� = Moving_time(From decided_position to dest_position)
			int pause_time_at_decided_position
				= time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(decided_position.first)
				- map->calc_necessary_time(decided_position.second.first, dest_position.second.first, creating_dummy->get_speed(decided_position.first));
			//��~���Ԃ̃Z�b�g
			creating_dummy->set_pause_time(decided_position.first, pause_time_at_decided_position);

			//�r���ړI�n���玟�̒�~�n�_��path�����߂�
			linear_interpolation_of_path_between_positions(decided_position.second.first, dest_position.second.first, &phase_id, &dest_rest_time);
		}

		//�V���~���[�V�����I���܂ł̎c��̒�~�ʒu��K���Ɍ��߂�
		//�����ɐݒ肵���ق����ǂ����C�Ƃ肠�����C�ŏI�n�_�̂�
		while (phase_id <= time_manager->phase_count())
		{
			double length_of_rect = 0.005;//�K�؂Ȕ͈͂̈ܓx�o�x�̑I�𕝂�����

			Entity::MobileEntity<Geography::LatLng>::node_pos_info now_poi = creating_dummy->read_node_pos_info_of_phase(phase_id);
			int next_arrive_time = time_manager->time_of_phase(time_manager->phase_count()) - time_manager->time_of_phase(phase_id);

			int distance = (next_arrive_time - requirement->max_pause_time) * creating_dummy->get_speed(phase_id);

			double angle_of_positions = 45;// generator.uniform_distribution(0, 90) or (270,360);

			Geography::LatLng next_candidate_poi_position_range
				= Geography::GeoCalculation::calc_translated_point(*now_poi.second, distance, angle_of_positions);

			Graph::Rectangle<Geography::LatLng> range(next_candidate_poi_position_range.lat(), next_candidate_poi_position_range.lng() - length_of_rect, next_candidate_poi_position_range.lat() - length_of_rect, next_candidate_poi_position_range.lng());
			//����POI�̌���
			std::vector<std::shared_ptr<Map::BasicPoi const>> candidate_pois_list = map->find_pois_within_boundary(range);
			std::random_device device2;
			std::mt19937_64 generator2(device2());
			std::shuffle(candidate_pois_list.begin(), candidate_pois_list.end(), generator2);
			std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator next_poi = candidate_pois_list.begin();

			//decided_position�̒�~���Ԃ����߂�
			//��~���� = Moving_time(From decided_position to dest_position)
			int pause_time_at_decided_position
				= next_arrive_time
				- map->calc_necessary_time(now_poi.first, (*next_poi)->get_id(), creating_dummy->get_speed(phase_id));
			//��~���Ԃ̃Z�b�g
			creating_dummy->set_pause_time(phase_id, pause_time_at_decided_position);

			//�r���ړI�n���玟�̒�~�n�_��path�����߂�
			linear_interpolation_of_path_between_positions(now_poi.first, (*next_poi)->get_id(), &phase_id, &dest_rest_time);
		}
	}


	///<summary>
	/// ������ (����͓��ɂ�邱�Ƃ͂Ȃ�)
	///</summary>
	void KatoBachelorMethod::initialize()
	{

	}


	///<summary>
	/// ��������Ď�@�̊j�ɂȂ镔��
	///</summary>
	void KatoBachelorMethod::decide_dummy_positions()
	{
		for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++)
		{
			creating_dummy = entities->get_dummy_by_id(dummy_id);

			decide_base_positions_and_arrive_time(dummy_id);// �������_�~�[(k�Ԗ�)�̊�n�_����ъ�n�_�������Ԃ̌���
			decide_share_positions_and_arrive_time(dummy_id);// �������_�~�[(k�Ԗ�)�̋��L�n�_����ы��L�n�_�������Ԃ̌���
			decide_destination_on_the_way(dummy_id);// �������_�~�[(k�Ԗ�)�̈ړ��o�H�̌���
		}
	}

	void KatoBachelorMethod::run()
	{
		//�����Ŏ��s���Ԃ̌v�����J�n
		timer->start();

		//������
		initialize();

		//���������s����(�e�����̃_�~�[�ʒu���v�Z����)
		decide_dummy_positions();

		//�����Ōv�����I��
		timer->end();

		//�I������
		terminate();
	}

}
