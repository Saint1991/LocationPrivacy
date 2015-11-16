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
		grid_list(std::vector<Grid>((time_manager->phase_count() / requirement->interval_of_base_phase))),
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
	/// start_phase����end_phase�܂ł�cell_id�̂����C�G���e�B�e�B���������ɕ��בւ���list�Ŏ擾���܂�
	/// pair��(cell_id,enntities_num)
	///</summary>
	std::list<std::pair<int, int>> KatoBachelorMethod::get_cell_id_list_order_by_entities_num(std::vector<int>& total_entity_num) const
	{
		int cell_id = 0;
		std::list<std::pair<int, int>> cell_list_of_min_entities_num;
		for (std::vector<int>::const_iterator iter = total_entity_num.begin(); iter != total_entity_num.end(); iter++, cell_id++) {
			cell_list_of_min_entities_num.push_back(std::make_pair(cell_id, *iter));
		}
		cell_list_of_min_entities_num.sort([](const std::pair<int, int>& cell_id1, const std::pair<int, int>& cell_id2) {
			return cell_id1.second < cell_id2.second;
		});

		return cell_list_of_min_entities_num;
	}


	///<summary>
	/// start_phase����end_phase�܂łŁCmin_cell_id�̃Z���ŃG���e�B�e�B���������ƂȂ�base_phase��list�Ŏ擾
	/// pair��(base_phase,entities_num)
	///</summary>
	std::list<std::pair<int, int>> KatoBachelorMethod::get_phase_of_min_cell_id_list_ordered_by_entities_num(std::vector<std::vector<int>>& entities_num_table, int min_cell_id, int start, int end) const
	{
		std::list<std::pair<int, int>> base_phase_list_of_min_entities_num;
		for (int base_phase_id = start; base_phase_id < end; base_phase_id++) {
			base_phase_list_of_min_entities_num.push_back(std::make_pair(base_phase_id, entities_num_table.at(base_phase_id).at(min_cell_id)));
		}

		base_phase_list_of_min_entities_num.sort([](const std::pair<int, int>& base_phase_id1, const std::pair<int, int>& base_phase_id2) {
			return base_phase_id1.second < base_phase_id2.second;
		});
		return base_phase_list_of_min_entities_num;
	}


	///<summary>
	/// Rectangle�Ɋ܂܂��POI�̃��X�g���擾
	/// boundary����poi��������Ȃ��ꍇ�͔͈͂��L���čĒT��
	///</summary>
	std::vector<std::shared_ptr<Map::BasicPoi const>> KatoBachelorMethod::get_candidate_pois_list(Graph::Rectangle<Geography::LatLng>& boundary) {
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
			//�S�̂̒�~���Ԃ���C�O��̓�������������������~���Ԃ�����������
		int rest_pause_time = creating_dummy->get_pause_time(*phase_id) - *dest_rest_time;

		//�c��̒�~���Ԃ�phase�Ɋ��Z���Cphase�����߂�D
		div_t variable_of_converted_pause_time_to_phase = std::div(rest_pause_time, requirement->service_interval);

		//��~���ԕ��C�ephase�ɒ�~�ꏊ�ƈړ����x(0)��o�^
		for (int i = 0; i < variable_of_converted_pause_time_to_phase.quot; i++)
		{
			(*phase_id)++;
			creating_dummy->set_position_of_phase(*phase_id, source, map->get_static_poi(source.id())->data->get_position());
			creating_dummy->set_speed(*phase_id, 0);
		}

		std::vector<Graph::MapNodeIndicator> shortests_path_between_pois = map->get_shortest_path(source, destination);
		std::vector<Graph::MapNodeIndicator>::iterator path_iter = shortests_path_between_pois.begin();//path���������邽�߂�index

		double pause_position_speed = creating_dummy->get_speed(*phase_id - variable_of_converted_pause_time_to_phase.quot);//��~�ʒu�̏o�����x��ێ����Ă���

		//source����̋���
		//�ŏ�������~���Ԃ�phase�Ɋ��Z�������̗]���time�Ƃ��C����ȊO��service_interval��time�Ƃ��āC���ݒn���狁�߂����n�_��distance���v�Z
		//���x��phase�Ŗ��߂�O���Q�Ƃ��Ȃ���΂Ȃ�Ȃ����Ƃɒ���
		int now_time = requirement->service_interval - variable_of_converted_pause_time_to_phase.rem;
		int total_time_from_source_to_destination = map->calc_necessary_time(source, destination, pause_position_speed);
		Graph::MapNodeIndicator nearest_position = source;
		//path���쐬�D�ꏊ�͈�ԋ߂�intersection���m�Ő��`��Ԃ���DMapNodeIndicator��Type��INVALID�Ƃ���D
		while (now_time < total_time_from_source_to_destination)
		{
			//�ŏ��͒�~���Ԃ�phase�Ɋ��Z�����Ƃ��̗]�蕪��distance�Ƃ��āC�ŒZ�H�̒��ň�ԋ߂�intersection��T���C���`��Ԃ���D
			//double total_path_length = map->shortest_distance(source, *path_iter);
			while (now_time > map->calc_necessary_time(source, *path_iter, pause_position_speed))
			{
				nearest_position = *path_iter;
				path_iter++;
			}

			int distance = now_time * pause_position_speed;
			int distance_between_nearest_intersection_and_arrive_position = distance - map->shortest_distance(source, nearest_position);
			Geography::LatLng nearest_latlng
				= nearest_position.type() == Graph::NodeType::POI ? map->get_static_poi(nearest_position.id())->data->get_position() : *map->get_static_node(nearest_position.id())->data;
			Geography::LatLng next_nearest_latlang
				= (*path_iter).type() == Graph::NodeType::POI ? map->get_static_poi((*path_iter).id())->data->get_position() : *map->get_static_node((*path_iter).id())->data;
			double angle = Geography::GeoCalculation::lambert_azimuth_angle(nearest_latlng, next_nearest_latlang);

			Geography::LatLng arrive_position = Geography::GeoCalculation::calc_translated_point(nearest_latlng, distance_between_nearest_intersection_and_arrive_position, angle);

			(*phase_id)++;
			creating_dummy->set_position_of_phase(*phase_id, Graph::MapNodeIndicator(Graph::NodeType::OTHERS, Graph::NodeType::OTHERS), arrive_position);
			creating_dummy->set_speed(*phase_id, pause_position_speed);

			now_time += requirement->service_interval;
		}

		//destination�̂Ƃ���܂ŕ⊮�ł�����Crest_time��ێ����Ă����I
		int time_between_arrive_position_and_dest_position = now_time - total_time_from_source_to_destination;
		*dest_rest_time
			= time_between_arrive_position_and_dest_position == requirement->service_interval ? 0 : time_between_arrive_position_and_dest_position;
		//�ړI�n�̓o�^
		//speed�͕ʓr�ݒ�̂��ߕs�v
		(*phase_id)++;
		creating_dummy->set_position_of_phase(*phase_id, destination, map->get_static_poi(destination.id())->data->get_position());
	}




	///<summary>
	/// �������_�~�[(k�Ԗ�)�̊�n�_����ъ�n�_�������Ԃ̌���
	///</summary>
	void KatoBachelorMethod::decide_base_positions_and_arrive_time(int dummy_id)
	{
		//phase�͂��������̈��phase0�ɂ��ݒ肵�����ꍇ
		//1:�����l��0�ɂ���
		//2:grid_list��entities_num_table��vector�̌���(time_manager->phase_count()/requirement->interval_of_base_phase)+1�ɂ���
		//3:real_phase��base_phase+1�łȂ͂��Cbase_phase�ɂ���
		int phase = requirement->interval_of_base_phase;
		const int GRID_TOTAL_NUM = CELL_NUM_ON_SIDE*CELL_NUM_ON_SIDE;//�O���b�h�̐�

																	 //�e�O���b�h�̊e�t�F�C�Y�ɂ�����entities�̐����L�����邽�߂�table(���I�z��)�̍쐬
		std::vector<std::vector<int>> entities_num_table(GRID_TOTAL_NUM, std::vector<int>((time_manager->phase_count() / requirement->interval_of_base_phase), 0));

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
		int start_of_cycle = 0;

		//�I��������entities_table��size�ɂȂ�܂�!
		for (int end_of_cycle = requirement->cycle_of_interval_of_base_num; end_of_cycle <= entities_num_table.at(0).size();
		start_of_cycle += requirement->cycle_of_interval_of_base_num, end_of_cycle += requirement->cycle_of_interval_of_base_num)
		{
			//�e�Z����start~end�̃G���e�B�e�B�̍��v(�\�̍s�̘a���v�Z���Ă��邱�Ƃɑ���)
			std::vector<int> total_entity_num_at_interval_phase = get_total_num_of_each_cell_at_interval_phase(entities_num_table, start_of_cycle, end_of_cycle);
			//start_phase����end_phase�܂łŁC�G���e�B�e�B���������ƂȂ�Z��id��list�ŋ��߂�D
			std::list<std::pair<int, int>> min_cell_id_list = get_cell_id_list_order_by_entities_num(total_entity_num_at_interval_phase);

			//min_cell_id�̃Z���ŃG���e�B�e�B���������ƂȂ�base_phase��list�Ŏ擾
			Math::Probability generator;
			int base_phase = generator.uniform_distribution(start_of_cycle, end_of_cycle - 1);
			if (end_of_cycle == 4) base_phase = 2;
			
			//base_phase��interval_of_base_phase�̒��̐��Ȃ̂ŁC���ۂ�phase�͕�
			//grid_list�̂�base_phase���g���I
			int real_phase = (base_phase + 1) * requirement->interval_of_base_phase;

			//�擾����cell_id,phase�ɂ������~�n�_���擾
			//��l���z�Ń����_���擾(������Ȃ������ꍇ�̍L����傫���͍l�������ق����ǂ���������Ȃ�)
			Graph::Rectangle<Geography::LatLng> cell = grid_list.at(base_phase).at(min_cell_id_list.front().first);
			std::vector<std::shared_ptr<Map::BasicPoi const>> poi_within_base_point_grid = get_candidate_pois_list(cell);
			std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_within_base_point_grid.begin();

			//��ԍŏ��̂ݓ��B�\�����l�������ɒ�~�n�_�����肷��D
			if (creating_dummy->find_previous_fixed_position(time_manager->phase_count()).first == INVALID) {
				creating_dummy->set_position_of_phase(real_phase, Graph::MapNodeIndicator((*poi)->get_id()), (*poi)->data->get_position());
				creating_dummy->set_random_speed(real_phase, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);
			}
			//��ӏ��ڈȍ~�̊�n�_�́C�ȑO�̊�n�_���瓞�B�\���𒲂ׂ��̂��Ɍ��肷��D
			else
			{
				std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_base_info = creating_dummy->find_previous_fixed_position(real_phase);
				int base_time_limit = time_manager->time_of_phase(real_phase) - time_manager->time_of_phase(previous_base_info.first) - requirement->min_pause_time;

				//�̈����POI��5��܂ŒT�����C���B�\�Ȃ炻�̓_���̗p
				//������Ȃ��ꍇ�͈Ⴄ�̈�(cell)���Q�Ƃ��čĒT��
				auto serach_poi = [&](int flag_id = 0) {
					for (int flag = 0; flag < 5; flag++) {
						if (map->is_reachable(previous_base_info.second.first, Graph::MapNodeIndicator((*poi)->get_id()), creating_dummy->get_speed(previous_base_info.first), base_time_limit)) {
							return flag_id + 1;
						}
						else {
							poi++;
						}
					}
					return flag_id;
				};

				for (std::list<std::pair<int, int>>::iterator min_cell_iter = min_cell_id_list.begin(); min_cell_iter != min_cell_id_list.end(); min_cell_iter++) {
					if (serach_poi() == 0) {
						cell = grid_list.at(base_phase).at((*min_cell_iter).first);
						poi_within_base_point_grid = get_candidate_pois_list(cell);
						poi = poi_within_base_point_grid.begin();
					}
					else {
						break;
					}
				}

				creating_dummy->set_position_of_phase(real_phase, Graph::MapNodeIndicator((*poi)->get_id()), (*poi)->data->get_position());
				creating_dummy->set_random_speed(real_phase, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);
			}
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
			if (iter->first >= dummy_id) continue;

			//�����Ώۂ�ID(�����񐔍ŏ�)
			int cross_target = iter->first;
			int target_phase = INVALID;

			//BasicUser��Dummy���p�����Ă���̂Ń|�C���^����͉\
			//target���������郆�[�Uor�_�~�[
			//target_phases�͌������ݒ肳��Ă��Ȃ�phase�̈ꗗ
			std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> target = cross_target == 0 ? entities->get_user() : entities->get_dummy_by_id(cross_target);
			std::vector<int> target_phases = target->find_cross_not_set_phases_of_poi();

			//��n�_�ɐݒ肳��Ă���phase�͍폜����D
			int interval_of_base_phase = requirement->interval_of_base_phase;
			auto iterNewEnd = std::remove_if(target_phases.begin(), target_phases.end(), [interval_of_base_phase](int phase) {
				return phase % interval_of_base_phase == 0;
			});
			target_phases.erase(iterNewEnd, target_phases.end());


			//�������ݒ�̎������烉���_���ɋ��L�n�_�ݒ�����݂�
			std::random_device device;
			std::mt19937_64 generator(device());
			std::shuffle(target_phases.begin(), target_phases.end(), generator);

			//�t�F�[�Y��0�łȂ����������ݒ�̃t�F�[�Y�Ɍ����n�_��ݒ肷��D
			for (std::vector<int>::const_iterator target_phase = target_phases.begin(); target_phase != target_phases.end(); target_phase++) {
				int share_phase = *target_phase;
				std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>> share_position = target->read_node_pos_info_of_phase(share_phase);

				//�������_�~�[�̊��ɒ�~�ʒu�����肵�Ă���t�F�[�Y�������L�t�F�[�Y���傫���ꍇ
				if (creating_dummy->find_previous_fixed_position(time_manager->phase_count()).first <= share_phase)
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_info = creating_dummy->find_previous_fixed_position(time_manager->phase_count());
					int previous_time_limit = time_manager->time_of_phase(share_phase) - time_manager->time_of_phase(previous_info.first) - requirement->min_pause_time;

					//���L�ꏊ�ɓ��B�\�Ȃ炻�̈ʒu��ݒ肵�C���B�s�\�Ȃ�΂�����x�ʂ̃t�F�[�Y������
					if (previous_time_limit < 0 || !map->is_reachable(previous_info.second.first, share_position.first, creating_dummy->get_speed(previous_info.first), previous_time_limit)) {
						continue;
					}
				}
				//�������_�~�[�̊��ɒ�~�ʒu�����肵�Ă���t�F�[�Y�������L�t�F�[�Y���������ꍇ
				//��������
				else if (creating_dummy->get_speed(0) == 0 && creating_dummy->find_next_fixed_position(0).first >= share_phase)
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> next_info = creating_dummy->find_next_fixed_position(0);
					int next_time_limit = time_manager->time_of_phase(next_info.first) - time_manager->time_of_phase(share_phase) - requirement->min_pause_time;

					if (next_time_limit < 0 || !map->is_reachable(share_position.first, next_info.second.first, target->get_speed(share_phase), next_time_limit)) {
						continue;
					}
				}
				//�������_�~�[�̊��ɒ�~�ʒu�����肵�Ă���t�F�[�Y�̊Ԃɂ���ꍇ
				else
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_info = creating_dummy->find_previous_fixed_position(share_phase);
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> next_info = creating_dummy->find_next_fixed_position(share_phase);
					//time_limit��previous�ł̒�~���Ԃ��l�����Ȃ���΂Ȃ�Ȃ����Ƃɒ���
					int previous_time_limit = time_manager->time_of_phase(share_phase) - time_manager->time_of_phase(previous_info.first) - requirement->min_pause_time;
					int next_time_limit = time_manager->time_of_phase(next_info.first) - time_manager->time_of_phase(share_phase) - requirement->min_pause_time;

					if (previous_time_limit < 0 || next_time_limit < 0) continue;
					if (!map->is_reachable(previous_info.second.first, share_position.first, creating_dummy->get_speed(previous_info.first), previous_time_limit)
						&& !map->is_reachable(share_position.first, next_info.second.first, target->get_speed(share_phase), next_time_limit)) {
						//�����ň�UPOI��T���Ȃ������Ƃ��s���I
						continue;
					}
				}

				//Dmincross = += 1;
				//�������̃_�~�[�̌����� += 1;
				creating_dummy->set_crossing_position_of_phase(share_phase, share_position.first, *share_position.second);
				creating_dummy->set_speed(share_phase, target->get_speed(share_phase));
				creating_dummy->set_random_speed(share_phase, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);
				//target�̌�����+1
				target->register_as_cross_position(share_phase);
				break;

			}
			//���[�U�y�ѐ����ς݃_�~�[�̕��ό����񐔂����C�������_�~�[�̌����񐔂������Ȃ�܂ŋ��L�n�_��ݒ肷��
			int check_cross_count = creating_dummy->get_cross_count();
			int check_all_entities_total_cross_count = entities->get_all_entities_total_crossing_count();

			//�ŏ������C�������L�n�_���ݒ�ł��Ȃ�������break������
			if (check_cross_count == check_all_entities_total_cross_count) break;
			if (check_cross_count > check_all_entities_total_cross_count / (requirement->dummy_num + 1)) break;
		}
	}



	///<summary>
	/// �������_�~�[(k�Ԗ�)�̈ړ��o�H�̌���
	///</summary>
	void KatoBachelorMethod::decide_destination_on_the_way(int dummy_id)
	{
		int phase_id = 0;
		Math::Probability generator;
		//�������_�~�[�̊��Ɍ��܂��Ă��钆�ōŏ��̒n�_���擾
		std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> dest_position = creating_dummy->find_next_fixed_position(phase_id);

		//------------------------------------�������ʒu�̌��聫------------------------------------------------------//

		//creating_dummy->set_pause_time(0, requirement->max_pause_time);//����phase�̒�~����
		creating_dummy->set_random_speed(phase_id, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);//����phase��speed�̌���

		//�������_�~�[�̃v�����̒��ŁC��ԍŏ��̏ꏊ����0�b�܂ł͈̔�(�ő��~���Ԃ��l��)�œ����ł���POI���擾
		//��U���X�g�Ŏ擾���Ă���C���̒����烉���_���őI��
		double distance = creating_dummy->get_speed(phase_id) * (time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(phase_id));
		Geography::LatLng candidate_init_latlng = Geography::GeoCalculation::calc_translated_point(*dest_position.second.second, distance, M_PI * 0.5);
		//POI��T�����钷���`���擾�D�ړI�n�ɋ߂Â��������l��
		double top = dest_position.second.second->lat() - candidate_init_latlng.lat() > 0 ? dest_position.second.second->lat() : candidate_init_latlng.lat();
		double left = dest_position.second.second->lng() - candidate_init_latlng.lng() > 0 ? candidate_init_latlng.lng() : dest_position.second.second->lng();
		double bottom = dest_position.second.second->lat() - candidate_init_latlng.lat() < 0 ? dest_position.second.second->lat() : candidate_init_latlng.lat();
		double right = dest_position.second.second->lng() - candidate_init_latlng.lng() < 0 ? candidate_init_latlng.lng() : dest_position.second.second->lng();
		Graph::Rectangle<Geography::LatLng> rect1(top, left, bottom, right);

		std::vector<std::shared_ptr<Map::BasicPoi const>> init_pois_list = get_candidate_pois_list(rect1);
		std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator init_poi = init_pois_list.begin();
		int init_time_limit = time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(phase_id) - requirement->min_pause_time;

		while (!map->is_reachable(Graph::MapNodeIndicator((*init_poi)->get_id()), dest_position.second.first, creating_dummy->get_speed(phase_id), init_time_limit)) {
			init_poi++;
		}

		//PPout��<position, start, pause��init>��ǉ�;
		creating_dummy->set_position_of_phase(phase_id, Graph::MapNodeIndicator((*init_poi)->get_id()), (*init_poi)->data->get_position());

		//------------------------------------�������ʒu�̌��聪------------------------------------------------------//

		//�����ʒu�ȍ~������ɒ�~�ʒu����ς݂�phase�܂ł̒�~�n�_�̌���
		int dest_rest_time = 0;
		while (phase_id < creating_dummy->find_previous_fixed_position(time_manager->phase_count()).first)
		{
			std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> decided_position = creating_dummy->find_previous_fixed_position(dest_position.first);
			int time_between_decided_and_dest_position = time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(decided_position.first);
			//��_�Ԃ����ɋ߂��C���C�A���Œ�~�n�_�����܂��Ă��Ȃ��ꍇ�r����~�ʒu��ݒ�
			//already��dest�̓������Ԃ̍�>decided��dest�̈ړ����ԁ{MaxPauseTime
			//if (time_between_decided_and_dest_position > 2* requirement->max_pause_time) throw std::invalid_argument("Pause Time will be to long!!");
			if (time_between_decided_and_dest_position > map->calc_necessary_time(decided_position.second.first, dest_position.second.first, creating_dummy->get_speed(decided_position.first)) + requirement->max_pause_time) {
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

				std::vector<std::shared_ptr<Map::BasicPoi const>> on_the_way_pois_list = get_candidate_pois_list(rect2);
				std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi_on_the_way = on_the_way_pois_list.begin();

				//�r���ړI�n�̑��x��ݒ肷��
				//PauseTime���܂����܂�Ȃ��̂ŁC�Z�b�g�͂��Ȃ��D
				double on_the_way_speed = generator.uniform_distribution(requirement->average_speed_of_dummy - 0.5 * requirement->speed_range_of_dummy, requirement->average_speed_of_dummy + 0.5 * requirement->speed_range_of_dummy);

				int total_pause_time_at_decided_and_dest_position
					= time_between_decided_and_dest_position
					- map->calc_necessary_time(decided_position.second.first, (*poi_on_the_way)->get_id(), creating_dummy->get_speed(decided_position.first))
					- map->calc_necessary_time((*poi_on_the_way)->get_id(), dest_position.second.first, on_the_way_speed);

				int percentage_of_spilit_pause_time = generator.uniform_distribution(45, 55);
				double rate_of_spilit_pause_time = percentage_of_spilit_pause_time / 100.0;
				int first_pause_time = total_pause_time_at_decided_and_dest_position * rate_of_spilit_pause_time;
				int second_pause_time = total_pause_time_at_decided_and_dest_position - first_pause_time;

				//����ς݂̓_�̒�~���Ԃ�ݒ�
				creating_dummy->set_pause_time(decided_position.first, first_pause_time);

				//�r���ړI�n�܂ł�path�����߂�
				linear_interpolation_of_path_between_positions(decided_position.second.first, (*poi_on_the_way)->get_id(), &phase_id, &dest_rest_time);

				//�r���ړI�n�̑��x�̃Z�b�g
				creating_dummy->set_speed(phase_id, on_the_way_speed);

				//�r���ړI�n�̒�~���Ԃ̃Z�b�g
				creating_dummy->set_pause_time(phase_id, second_pause_time);

				//decided_position�̍X�V
				decided_position = creating_dummy->find_previous_fixed_position(dest_position.first);

				//�ړI�n�܂ł�path�����߂�
				linear_interpolation_of_path_between_positions(decided_position.second.first, dest_position.second.first, &phase_id, &dest_rest_time);

				//�ړI�n�̍X�V
				dest_position = creating_dummy->find_next_fixed_position(phase_id);
			}
			else {
				//decided_position�̒�~���Ԃ����߂�
				//��~���� = Moving_time(From decided_position to dest_position)
				int pause_time_at_decided_position
					= time_between_decided_and_dest_position - map->calc_necessary_time(decided_position.second.first, dest_position.second.first, creating_dummy->get_speed(phase_id));

				//��~���Ԃ̃Z�b�g
				creating_dummy->set_pause_time(phase_id, pause_time_at_decided_position);

				//�r���ړI�n���玟�̒�~�n�_��path�����߂�
				linear_interpolation_of_path_between_positions(decided_position.second.first, dest_position.second.first, &phase_id, &dest_rest_time);

				//�ړI�n�̍X�V
				dest_position = creating_dummy->find_next_fixed_position(phase_id);

			}
		}

		//�V���~���[�V�����I���܂ł̎c��̒�~�ʒu��K���Ɍ��߂�
		//�����ɐݒ肵���ق����ǂ����C�Ƃ肠�����C�ŏI�n�_�̂�
		while (phase_id < time_manager->phase_count() - 1)
		{
			Entity::MobileEntity<Geography::LatLng>::node_pos_info now_poi = creating_dummy->read_node_pos_info_of_phase(phase_id);

			double length_of_rect = 0.005;//�K�؂Ȕ͈͂̈ܓx�o�x�̑I�𕝂�����
			int rest_phase_time = time_manager->time_of_phase(time_manager->phase_count() - 1) - time_manager->time_of_phase(phase_id);
			if ((rest_phase_time - requirement->min_pause_time) < 0) {
				for (int i = phase_id; i < time_manager->phase_count() - 1; i++)
				{
					phase_id++;
					creating_dummy->set_position_of_phase(phase_id, now_poi.first, *now_poi.second);
					creating_dummy->set_speed(phase_id, 0);
				}
				break;
			}
			else{
				//�����͓͂��͈͂Œ���
				int distance = 0.9 * (rest_phase_time - requirement->min_pause_time) * creating_dummy->get_speed(phase_id);
				double angle_of_positions = generator.uniform_distribution(-M_PI_2, M_PI_2);
				Geography::LatLng next_candidate_poi_position_range
					= Geography::GeoCalculation::calc_translated_point(*now_poi.second, distance, angle_of_positions);

				Graph::Rectangle<Geography::LatLng> range(next_candidate_poi_position_range.lat(), next_candidate_poi_position_range.lng() - length_of_rect, next_candidate_poi_position_range.lat() - length_of_rect, next_candidate_poi_position_range.lng());
				//����POI�̌���
				std::vector<std::shared_ptr<Map::BasicPoi const>> candidate_pois_list = get_candidate_pois_list(range);
				std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator next_poi = candidate_pois_list.begin();

				//decided_position�̒�~���Ԃ����߂�
				//��~���� = Moving_time(From decided_position to dest_position)
				int pause_time_at_decided_position
					= rest_phase_time
					- map->calc_necessary_time(now_poi.first, (*next_poi)->get_id(), creating_dummy->get_speed(phase_id));
				//��~���Ԃ̃Z�b�g
				creating_dummy->set_pause_time(phase_id, pause_time_at_decided_position);

				//�r���ړI�n���玟�̒�~�n�_��path�����߂�
				linear_interpolation_of_path_between_positions(now_poi.first, (*next_poi)->get_id(), &phase_id, &dest_rest_time);
		
			}
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
