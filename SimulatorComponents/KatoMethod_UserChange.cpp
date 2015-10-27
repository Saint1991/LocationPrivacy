#include "stdafx.h"
#include "KatoMethod_UserChange.h"

namespace Method
{
	
	///<summary>
	/// �R���X�g���N�^
	/// �����Simulator�ō쐬�����e����͂ւ̎Q�Ƃ�n��
	///</summary>
	KatoMethod_UserChange::KatoMethod_UserChange(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager, std::shared_ptr<Time::TimeSlotManager> revise_time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement>(map, user, requirement, time_manager),grid_list(std::vector<Grid>(time_manager->phase_count())),creating_dummy(nullptr),revise_time_manager(revise_time_manager)
	{
		/*
		for (int i = 0; i < revise_time_manager->phase_count(); i++)
		{
			std::shared_ptr<std::vector<time_t>> timesslot = revise_time_manager->time_of_phase(i);
		}
		*/
	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	KatoMethod_UserChange::~KatoMethod_UserChange()
	{
	}

	
	///<summary>
	/// T[s]���Ƃ̃O���b�h�̈���쐬
	/// grid_length�̓O���b�h�S�̂̒���
	///</summary>
	std::vector <Graph::Rectangle<Geography::LatLng>> KatoMethod_UserChange::make_grid(double grid_length, const Geography::LatLng& center, int cell_num_on_side)
	{
		double side_length = grid_length / cell_num_on_side;//�Z������̒����`�̒���
		//center�̎l�_�̍��W
		double top = center.y() + side_length * 1.5;
		double left = center.x() - side_length * 1.5;
		double bottom = center.y() + side_length * 0.5;
		double right = center.x() - side_length * 0.5;
		 
		std::vector<Graph::Rectangle<Geography::LatLng>> grid_list;//�O���b�h�S�̂��Ǘ����郊�X�g
		
		double base_left = left;//����̐����`��left�����[�v�̍ۂ̊�Ƃ���
		double base_right = right;//����̐����`��right�����[�v�̍ۂ̊�Ƃ���

		for (int i = 0; i < cell_num_on_side; i++)
		{
			for (int j = 0; j < cell_num_on_side; j++)
			{
				grid_list.push_back(Graph::Rectangle<Geography::LatLng>(top, left, bottom, right));
				right += side_length;
				left += side_length;
			}

			top -= side_length;
			bottom -= side_length;
			left = base_left;
			right = base_right;
		}

		return grid_list;
	}


	///<summary>
	/// �O���b�h�e�[�u����start_phase����end_phase�̃G���e�B�e�B�̍��v���擾
	///</summary>
	std::vector<int> KatoMethod_UserChange::get_total_num_of_each_cell_at_interval_phase(std::vector<std::vector<int>>& entities_table, int start_phase, int end_phase) {
		std::vector<int> total_entity_num_interval_phase;
		for (int grid_id = 0; grid_id < CELL_NUM_ON_SIDE*CELL_NUM_ON_SIDE; grid_id++)
		{
			int temp = 0;
			for (int phase = start_phase; phase < end_phase; phase++)
			{
				temp += entities_table.at(grid_id).at(phase);
			}
			total_entity_num_interval_phase.at(grid_id) = temp;
		}
		return total_entity_num_interval_phase;
	}


	
	///<summary>
	/// �O���b�h�e�[�u���̊e�O���b�h���Ƃ̑S�t�F�[�Y�̃G���e�B�e�B�̍��v���擾
	///</summary>
	std::vector<int> KatoMethod_UserChange::get_total_num_of_each_cell_at_all_phase(std::vector<std::vector<int>>& entities_table) {
		std::vector<int> total_entity_num_all_phase;
		for (int grid_id = 0; grid_id < CELL_NUM_ON_SIDE*CELL_NUM_ON_SIDE; grid_id++)
		{
			int temp = 0;
			for (int phase = 0; phase < time_manager->phase_count(); phase++)
			{
				temp += entities_table.at(grid_id).at(phase);
			}
			total_entity_num_all_phase.at(grid_id) = temp;
		}
		return total_entity_num_all_phase;
	}


	///<summary>
	/// Rectangle�Ɋ܂܂��POI�̃��X�g���擾
	///</summary>
	std::vector<std::shared_ptr<Map::BasicPoi const>> KatoMethod_UserChange::candidate_pois_list(const Graph::Rectangle<Geography::LatLng>& boundary) {
		std::vector<std::shared_ptr<Map::BasicPoi const>> candidate_pois_list = map->find_pois_within_boundary(boundary);
		std::random_device device;
		std::mt19937_64 generator(device());
		std::shuffle(candidate_pois_list.begin(), candidate_pois_list.end(), generator);

		return candidate_pois_list;
	}

	
	///<summary>
	/// �������_�~�[(k�Ԗ�)�̊�n�_����ъ�n�_�������Ԃ̌���
	///</summary>
	void KatoMethod_UserChange::decide_base_positions_and_arrive_time(int dummy_id)
	{
		int phase = 0;//phase
		const int GRID_TOTAL_NUM = CELL_NUM_ON_SIDE*CELL_NUM_ON_SIDE;//�O���b�h�̐�
		
		//�e�O���b�h�̊e�t�F�C�Y�ɂ�����entities�̐����L�����邽�߂�table(���I�z��)�̍쐬
		std::vector<std::vector<int>> entities_num_table(GRID_TOTAL_NUM, std::vector<int>(time_manager->phase_count(),0));
				
		//����phase�ɂ�����e�Z���ɑ��݂��郆�[�U�y�ѐ����ς݃_�~�[�̈ړ��o�H(��~�n�_)�̐�
		//�����Z����id�C�c��phase��\�����I�Q�����z��ŋL��
		//phase_interval�̊Ԋu�ŋL�^���Ă���
		//k-2�ڂ܂ł�table������Ă����āCk-1�ڂ��{�P���čX�V����΂��������ǂ�
		while (phase <= time_manager->phase_count())
		{
			std::shared_ptr<Geography::LatLng const> center = entities->get_average_position_of_phase(phase);//���S�ʒu�����߂�
			Grid grid = make_grid(requirement->required_anonymous_area, *center, CELL_NUM_ON_SIDE);//phase���ƂɃO���b�h���쐬
			grid_list.at(phase) = grid;//����phase��Grid
						
			int cell_id = 0;//�Z����id
			//����phase�̑S�ẴZ���́C�G���e�B�e�B�����v�Z(�\�̗���v�Z���邱�Ƃɑ���)
			for (std::vector<Graph::Rectangle<Geography::LatLng>>::iterator iter = grid.begin(); iter != grid.end(); iter++)
			{
				entities_num_table.at((cell_id++) - 1).at(phase) = entities->get_entity_count_within_boundary(phase, *iter);
			}
			phase += requirement->phase_interval;
		}

		//������phase�Őݒ肵�C���̎����x�[�X�œ����̈�m�ۂ̂��߂̒n�_���쐬
		//�Y����������̃t�F�[�Y�ɂ����āC�G���e�B�e�B���ŏ��ɂȂ�Z��id���擾
		//�������Cphase0�͏��O
		//int cycle_id = requirement->phase_interval*requirement->cycle_of_anonymous_area;
		int start_of_cycle = 1;//�����̍��[
		int end_of_cycle = start_of_cycle + requirement->cycle_of_anonymous_area;//�����̉E�[
		
		//�e�Z����start_phase����end_phase�̃G���e�B�e�B�̍��v(�\�̍s�̘a���v�Z���Ă��邱�Ƃɑ���)
		std::vector<int> total_entity_num_interval_phase = get_total_num_of_each_cell_at_interval_phase(entities_num_table, start_of_cycle, end_of_cycle);


		while (end_of_cycle < time_manager->phase_count()) {
			//start_phase����end_phase�܂łŁC�G���e�B�e�B�����ŏ��ƂȂ�Z��id�����߂�D
			std::vector<int>::iterator cell_iter = std::min_element(total_entity_num_interval_phase.begin(), total_entity_num_interval_phase.end());
			size_t min_cell_id = std::distance(total_entity_num_interval_phase.begin(), cell_iter);

			//min_cell_id�̃Z���ōŏ��ɂȂ�ŏ���phase���擾
			std::vector<int>::iterator phase_iter = std::min_element(entities_num_table.at(min_cell_id).begin(), entities_num_table.at(min_cell_id).end());
			int base_phase = std::distance(entities_num_table.at(min_cell_id).begin(), phase_iter);
			
			//�擾����cell_id,phase�ɂ������~�n�_���擾
			//��l���z�Ń����_���擾
			//poi���Ȃ��������̏ꍇ�������l�����K�v��������Ȃ�
			Graph::Rectangle<Geography::LatLng> cell = grid_list.at(base_phase).at(min_cell_id);
			

			std::vector<std::shared_ptr<Map::BasicPoi const>> poi_within_base_point_grid = candidate_pois_list(cell);
			std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi = poi_within_base_point_grid.begin();
			//��ԍŏ��̂ݓ��B�\�����l�������ɒ�~�n�_�����肷��D
			if (creating_dummy->find_next_fixed_position(0).first == INVALID) {
				creating_dummy->set_position_of_phase(base_phase, Graph::MapNodeIndicator((*poi)->get_id()), (*poi)->data->get_position());
				creating_dummy->set_random_speed(base_phase, requirement->average_speed_of_dummy - 0.5*requirement->speed_range_of_dummy, requirement->average_speed_of_dummy + 0.5*requirement->speed_range_of_dummy);
			}
			//��ӏ��ڈȍ~�̊�n�_�́C�ȑO�̊�n�_���瓞�B�\���𒲂ׂ��̂��Ɍ��肷��D
			else
			{
				std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_base_info = creating_dummy->find_previous_fixed_position(base_phase);
				time_t base_time_limit = time_manager->time_of_phase(base_phase) - time_manager->time_of_phase(previous_base_info.first) - requirement->max_pause_time;
				//���B�\�łȂ��ꍇ�͕ʂ�poi�ɂ���D
				while (!map->is_reachable(previous_base_info.second.first, Graph::MapNodeIndicator((*poi)->get_id()), creating_dummy->get_speed(previous_base_info.first), base_time_limit)) {
					poi++;
				}
				/*
				���������ŁC�Y������poi���Ȃ�������C���ɐ��̏��Ȃ��Z���̗̈���Q�Ƃ���Ƃ�����O������������K�v������D
				*/
				creating_dummy->set_position_of_phase(base_phase, Graph::MapNodeIndicator((*poi)->get_id()), (*poi)->data->get_position());
				creating_dummy->set_random_speed(base_phase, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);
			}
		
			start_of_cycle += requirement->cycle_of_anonymous_area;
			end_of_cycle += requirement->cycle_of_anonymous_area;
		}
	}

	
	
	
	///<summary>
	/// �������_�~�[(k�Ԗ�)�̋��L�n�_����ы��L�n�_�������Ԃ̌���
	///</summary>
	void KatoMethod_UserChange::decide_share_positions_and_arrive_time(int dummy_id)
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
					time_t previous_time_limit = time_manager->find_phase_of_time(share_phase) - time_manager->find_phase_of_time(previous_info.first) - creating_dummy->get_pause_time(previous_info.first);
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
					time_t next_time_limit = time_manager->find_phase_of_time(next_info.first) - time_manager->find_phase_of_time(share_phase) - creating_dummy->get_pause_time(share_phase);
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
					time_t previous_time_limit = time_manager->find_phase_of_time(share_phase) - time_manager->find_phase_of_time(previous_info.first) - creating_dummy->get_pause_time(previous_info.first);
					time_t next_time_limit = time_manager->find_phase_of_time(next_info.first) - time_manager->find_phase_of_time(share_phase) -  creating_dummy->get_pause_time(share_phase);

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
			if(creating_dummy->get_cross_count() > entities->get_all_entities_total_crossing_count() / dummy_id) break;
		}
	}


	
	///<summary>
	/// �������_�~�[(k�Ԗ�)�̈ړ��o�H�̌���
	///</summary>
	void KatoMethod_UserChange::decide_destination_on_the_way(int dummy_id)
	{
		Graph::Rectangle<Geography::LatLng> rect(1.0, 1.0, 1.0, 1.0);
		
		//�������_�~�[�̊��Ɍ��܂��Ă��钆�ōŏ��̒n�_���擾
		std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> dest_position = creating_dummy->find_next_fixed_position(0);

		//------------------------------------�������ʒu�̌��聫------------------------------------------------------//

		creating_dummy->set_pause_time(0, 0);//����phase�̒�~����
		creating_dummy->set_random_speed(0, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);//����speed�̌���

		//�������_�~�[�̃v�����̒��ŁC��ԍŏ��̏ꏊ����0�b�܂ł͈̔�(�ő��~���Ԃ��l��)�œ����ł���POI���擾
		//��U���X�g�Ŏ擾���Ă���C���̒����烉���_���őI��
		double distance = creating_dummy->get_speed(0) * (time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(0));
		Geography::LatLng candidate_init_latlng = Geography::GeoCalculation::calc_translated_point(*dest_position.second.second, distance, M_PI);
		//POI��T�����钷���`���擾�D�ړI�n�ɋ߂Â��������l��
		double top = dest_position.second.second->lat() - candidate_init_latlng.lat() > 0 ? dest_position.second.second->lat() : candidate_init_latlng.lat();
		double left = dest_position.second.second->lng() - candidate_init_latlng.lng() > 0 ? candidate_init_latlng.lng() : dest_position.second.second->lng();
		double bottom = dest_position.second.second->lat() - candidate_init_latlng.lat() < 0 ? dest_position.second.second->lat() : candidate_init_latlng.lat();
		double right = dest_position.second.second->lng() - candidate_init_latlng.lng() < 0 ? candidate_init_latlng.lng() : dest_position.second.second->lng();
		Graph::Rectangle<Geography::LatLng> rect1(top, left, bottom, right);

		std::vector<std::shared_ptr<Map::BasicPoi const>> init_pois_list = candidate_pois_list(rect1);
		std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator init_poi = init_pois_list.begin();
		time_t init_time_limit = time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(0) - creating_dummy->get_pause_time(0);

		while (!map->is_reachable(Graph::MapNodeIndicator((*init_poi)->get_id()), dest_position.second.first, creating_dummy->get_speed(0), init_time_limit)) {
			init_poi++;
		}

		//PPout��<position, start, pause��init>��ǉ�;
		creating_dummy->set_position_of_phase(0, Graph::MapNodeIndicator((*init_poi)->get_id()), (*init_poi)->data->get_position());

		//------------------------------------�������ʒu�̌��聪------------------------------------------------------//
		
		//�����ʒu�ȍ~������ɒ�~�ʒu����ς݂�phase�܂ł̒�~�n�_�̌���
		int phase_id = 1;
		while (phase_id < creating_dummy->find_previous_fixed_position(time_manager->phase_count()).first)
		{
			std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> already_decided_position = creating_dummy->find_previous_fixed_position(dest_position.first);
			time_t time = time_manager->time_of_phase(already_decided_position.first) + requirement->max_pause_time + map->calc_necessary_time(already_decided_position.second.first, dest_position.second.first, creating_dummy->get_speed(already_decided_position.first)) ;
			//position(already_deceded)��position(dest_id)�ɓ��B�\�Ȃ�΁Cphase�ɂ�intersection��ǉ����C�����łȂ��Ȃ�r����~�ʒu��ݒ�
			//��_�Ԃ����ɋ߂��C���C�A���Œ�~�n�_�����܂��Ă��Ȃ��ꍇ�r����~�ʒu��ݒ�
			while (time > time_manager->time_of_phase(dest_position.first) && already_decided_position.first + 1 != dest_position.first) {
				//phase_id�Ԗڂ̒�~���Ԃƈړ����x������
				creating_dummy->set_random_pause_time(phase_id,requirement->min_pause_time,requirement->max_pause_time);
				creating_dummy->set_random_speed(phase_id,requirement->average_speed_of_dummy,requirement->speed_range_of_dummy);

				//position(phase_id)����position(dest)�Ԗڂ֓��B�\��POI����ЂƂ����_���Ŏ擾
				//�ړI�n�ɋ߂Â��������l�����Ȃ���΂Ȃ�Ȃ����Ƃɒ���
				double top = std::abs(already_decided_position.second.second->lat() - dest_position.second.second->lat());
				double left = already_decided_position.second.second->lng();
				double bottom = already_decided_position.second.second->lat();
				double right = std::abs(already_decided_position.second.second->lng() - dest_position.second.second->lng());
				Graph::Rectangle<Geography::LatLng> rect2(top, left, bottom, right);
			
				std::vector<std::shared_ptr<Map::BasicPoi const>> on_the_way_pois_list = candidate_pois_list(rect2);
				std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator poi_on_the_way = on_the_way_pois_list.begin();
				time_t time_limit = time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(phase_id) - creating_dummy->get_pause_time(phase_id);

				while (map->is_reachable(Graph::MapNodeIndicator((*poi_on_the_way)->get_id()), dest_position.second.first, creating_dummy->get_speed(phase_id), time_limit) == false) {
					poi_on_the_way++;
				}
				//��~�ʒu�̃Z�b�g
				creating_dummy->set_position_of_phase(phase_id, (*poi_on_the_way)->get_id(),(*poi_on_the_way)->data->get_position());
				phase_id++;
				//����_���X�V
				already_decided_position = creating_dummy->find_previous_fixed_position(phase_id);
				time_limit = time_manager->time_of_phase(dest_position.first) - time_manager->time_of_phase(already_decided_position.first) - creating_dummy->get_pause_time(already_decided_position.first);
			}


			//intersection��ǉ�
			//���̒�~�n�_�����܂��Ă���Ƃ���܂�intersection��⊮
			const std::vector<Graph::MapNodeIndicator> path = map->get_shortest_path(already_decided_position.second.first, dest_position.second.first);
			std::vector<Graph::MapNodeIndicator>::const_iterator iter = path.begin();
			for (; phase_id != dest_position.first; phase_id++,iter++)
			{
				Geography::LatLng position =
					iter->type() == Graph::NodeType::POI ? map->get_static_poi(iter->id())->data->get_position() : *map->get_static_node(iter->id())->data;
				creating_dummy->set_position_of_phase(phase_id, *iter, position);
				creating_dummy->set_speed(phase_id,creating_dummy->get_speed(phase_id-1));
				creating_dummy->set_pause_time(phase_id, 0);
			}
		
			//�r���ړI�n�̍X�V
			dest_position = creating_dummy->find_next_fixed_position(phase_id);
			phase_id++;
		}


		//�V���~���[�V�����I���܂ł̎c��̒�~�ʒu��K���Ɍ��߂�
		phase_id++;//�����̂��߂̃C���N�������g
		while (phase_id <= time_manager->phase_count())
		{
			creating_dummy->set_random_pause_time(phase_id, requirement->min_pause_time, requirement->max_pause_time);
			creating_dummy->set_random_speed(phase_id, requirement->average_speed_of_dummy, requirement->speed_range_of_dummy);
			std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> already_decided_position = dest_position;
			
			//time_limit�͑��x�Ǝ��Ԏg���ċ������K�v
			//�l�p�`�͈̔͂͗v�l�@
			time_t time_limit = time_manager->time_of_phase(phase_id + 1) - time_manager->time_of_phase(phase_id) - creating_dummy->get_pause_time(phase_id);
			double distance = creating_dummy->get_speed(phase_id)*time_limit;
			Geography::LatLng candidate_latlng = Geography::GeoCalculation::calc_translated_point(*dest_position.second.second, distance, M_PI);
			
			double top = dest_position.second.second->lat();
			double left = std::abs(dest_position.second.second->lng() - candidate_latlng.lng());
			double bottom = dest_position.second.second->lat() - candidate_latlng.lat();
			double right = dest_position.second.second->lng() + candidate_latlng.lng();
			Graph::Rectangle<Geography::LatLng> rect3(top, left, bottom, right);
						
			//positioni-1����positioni�Ԗڂ֓��B�\��POI����ЂƂ����_���Ŏ擾
			std::vector<std::shared_ptr<Map::BasicPoi const>> rest_pois_list = map->find_pois_within_boundary(rect3);
			std::vector<std::shared_ptr<Map::BasicPoi const>>::const_iterator rest_poi = rest_pois_list.begin();
			
			while (map->is_reachable(already_decided_position.second.first, Graph::MapNodeIndicator((*rest_poi)->get_id()), creating_dummy->get_speed(phase_id), time_limit) == false) {
				rest_poi++;
			}
			creating_dummy->set_position_of_phase(phase_id, (*rest_poi)->get_id(), (*rest_poi)->data->get_position());
			//����ʒu�̍X�V
			already_decided_position = creating_dummy->find_next_fixed_position(phase_id);
			phase_id++;
		}

	}

	///<summary>
	/// �_�~�[�̍s���v�������C������
	///</summary>
	void KatoMethod_UserChange::revise_dummy_movement_plan(int phase_id)
	{
		
	}


	///<summary>
	/// �_�~�[�̒�~���Ԃ̏C��
	///</summary>
	void KatoMethod_UserChange::revise_dummy_pause_time(int phase_id)
	{
		//�O�̒l�̕ێ�
		time_t previous_pause_time = creating_dummy->get_pause_time(phase_id);
		time_t previous_arrive_time = time_manager->time_of_phase(phase_id + 1);

		if (std::abs(time_to_change) > requirement->max_variation_of_pause_time)
		{
			time_t new_pause_time = time_to_change > 0 ? previous_pause_time + requirement->max_variation_of_pause_time : previous_pause_time - requirement->max_variation_of_pause_time;
			creating_dummy->set_pause_time(phase_id, new_pause_time);
		}
		else
		{
			creating_dummy->set_pause_time(phase_id, previous_pause_time + time_to_change);
		}

		if (requirement->max_pause_time < time_manager->time_of_phase(phase_id)) creating_dummy->set_pause_time(phase_id, requirement->max_pause_time);
		if (requirement->min_pause_time > time_manager->time_of_phase(phase_id)) creating_dummy->set_pause_time(phase_id, requirement->min_pause_time);
		
		//��~���Ԃ̕ω��ʂ����߂�
		time_t variation_of_pause_time = creating_dummy->get_pause_time(phase_id) - previous_pause_time;

		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//revise_time_manager->time_of_phase(i) += variation_of_pause_time;
		}
		if (time_manager->time_of_phase(phase_id+1) == previous_arrive_time + time_to_change) return;

		time_to_change -= variation_of_pause_time;//���̍s�͓��
	}

	/*
	///<summary>
	/// �_�~�[�̈ړ��o�H�̏C��
	///</summary>
	void KatoMethod_UserChange::revise_dummy_trajectory(int phase_id)
	{
		time_t previous_arrive_time = time_manager->time_of_phase(phase_id + 1);
		time_t tempT = 10000000000000000000;

		while (�S�Ẵg���W�F�N�g�����`�F�b�N���I����܂�) 
		{
			phase_id�̂Ƃ��̑��ݒn�_;
			time_manager->time_of_phase(phase_id + 1) = time_manager->time_of_phase(phase_id) + creating_dummy->get_pause_time(phase_id) + distance_of_Tri / creating_dummy->get_speed(phase_id);

		}

	}*/


	///<summary>
	/// �_�~�[�̍s�����x�̏C��
	///</summary>
	void KatoMethod_UserChange::revise_dummy_speed(int phase_id)
	{
		time_t previous_arrive_time = time_manager->time_of_phase(phase_id + 1);
		double previous_speed = creating_dummy->get_speed(phase_id);
		double distance = map->shortest_distance(creating_dummy->read_node_pos_info_of_phase(phase_id).first, creating_dummy->read_node_pos_info_of_phase(phase_id).first);
		time_t time = distance / (time_manager->time_of_phase(phase_id + 1)+time_to_change-time_manager->time_of_phase(phase_id));
		creating_dummy->set_speed(phase_id, time);

		if (std::abs(creating_dummy->get_speed(phase_id)-previous_speed) > requirement->max_variation_of_speed)
		{
			double new_speed = creating_dummy->get_speed(phase_id) - previous_speed > 0 ? previous_speed + requirement->max_variation_of_speed : previous_speed - requirement->max_variation_of_speed;
			creating_dummy->set_pause_time(phase_id, new_speed);
		}
		double max_speed = requirement->average_speed_of_dummy + 0.5* requirement->speed_range_of_dummy;
		double min_speed = requirement->average_speed_of_dummy - 0.5* requirement->speed_range_of_dummy;
		if (max_speed < creating_dummy->get_speed(phase_id)) creating_dummy->set_speed(phase_id, max_speed);
		if (min_speed > creating_dummy->get_speed(phase_id)) creating_dummy->set_speed(phase_id, min_speed);

		//time_manager->time_of_phase(phase_id + 1) = time_manager->time_of_phase(phase_id) + creating_dummy->get_pause_time(phase_id) + (time_t)(distance / creating_dummy->get_speed(phase_id));
		time_t variation_of_arrive_time = time_manager->time_of_phase(phase_id + 1) - previous_arrive_time;
		
		for (int i = phase_id + 1; i <= time_manager->phase_count(); i++)
		{
			//revise_time_manager->time_of_phase(i) += variation_of_pause_time;
		}
		if (time_manager->time_of_phase(phase_id + 1) == previous_arrive_time + time_to_change) return;

	}


	///<summary>
	/// �_�~�[�̒�~�ʒu�̏C��
	///</summary>
	void KatoMethod_UserChange::revise_dummy_pose_position(int phase_id)
	{

	}

	///<summary>
	/// ���[�U�̍s���v�����Ɋ܂܂���~�n�_�Ɍ������Ă��邩�ǂ������`�F�b�N
	///</summary>
	bool KatoMethod_UserChange::check_going_pause_position_in_plan()
	{
		return true;
	}
	
	///<summary>
	/// ���[�U�̍s���v�����ύX�̔��f
	///</summary>
	bool KatoMethod_UserChange::check_user_plan()
	{
		return true;
	}
	

	///<summary>
	/// ���[�U�̍s���v�����ύX�̔��f
	///</summary>
	void KatoMethod_UserChange::predict_user_plan()
	{

	}

	///<summary>
	///�@����n�_�ƈܓx�������ŁC������distance����Ă���_���擾
	///</summary>
	Geography::LatLng KatoMethod_UserChange::position_from_node_with_distance(Geography::LatLng position, double distance)
	{
		double lng = position.lng() - distance / (a*std::cos(position.lat()))*std::sqrt(1-(1-(b*b)/(a*a))*std::sin(position.lat())*std::sin(position.lat()));
		return Geography::LatLng(position.lat(), lng);
	}
	

	///<summary>
	/// ������ (����͓��ɂ�邱�Ƃ͂Ȃ�)
	///</summary>
	void KatoMethod_UserChange::initialize()
	{
		//���[�U�̓����̕ύX���V�������D
	}

	
	///<summary>
	/// ��������Ď�@�̊j�ɂȂ镔��
	///</summary>
	void KatoMethod_UserChange::decide_dummy_positions()
	{		
		for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++)
		{	
			creating_dummy = entities->get_dummy_by_id(dummy_id);
			
			decide_base_positions_and_arrive_time(dummy_id);// �������_�~�[(k�Ԗ�)�̊�n�_����ъ�n�_�������Ԃ̌���
			decide_share_positions_and_arrive_time(dummy_id);// �������_�~�[(k�Ԗ�)�̋��L�n�_����ы��L�n�_�������Ԃ̌���
			decide_destination_on_the_way(dummy_id);// �������_�~�[(k�Ԗ�)�̈ړ��o�H�̌���
		}
	}
	
	
	///<summary>
	/// ��������Ď�@�̊j�ɂȂ镔��.�_�~�[�̍s�����C������
	///</summary>
	void KatoMethod_UserChange::revise_dummy_positions()
	{
		time_t time_to_change = 0;// (�C�����)time_manager->time_of_phase(phase_id) - (�C���O��)time_manager->time_of_phase(phase_id);
		
		for (int phase_id = 0; phase_id < time_manager->phase_count(); phase_id++)
		{
			if(check_going_pause_position_in_plan()){
				if (!check_user_plan()) {
					predict_user_plan();//���̒�~�n�_�̓������Ԃ�\��
					revise_dummy_movement_plan(phase_id);
				}
			}
		}
									  


	}


	///<summary>
	/// ���肵���ʒu�����MTC���e��]���l���Z�o����
	///</summary>
	void KatoMethod_UserChange::evaluate()
	{

	}


	///<summary>
	/// ���ʂ̃t�@�C���ւ̃G�N�X�|�[�g
	///</summary>
	void KatoMethod_UserChange::export_results()
	{

	}


	///<summary>
	/// �I������ (����̓X�}�[�g�|�C���^�𗘗p���Ă���̂ŁC���ɂ�邱�Ƃ͂Ȃ�)
	///</summary>
	void KatoMethod_UserChange::terminate()
	{

	}

	void KatoMethod_UserChange::run()
	{
		//�����Ŏ��s���Ԃ̌v�����J�n
		timer->start();

		//������
		initialize();

		//���������s����(�e�����̃_�~�[�ʒu���v�Z����)
		decide_dummy_positions();

		//�����Ń��[�U�̍s���̗\����_�~�[�̍s�����C������
		revise_dummy_positions();

		//�����Ōv�����I��
		timer->end();

		//�ݒ肵���_�~�[�C���[�U�̈ʒu�����MTC�Ȃǂ̕]���w�W���v�Z����
		evaluate();

		//���s���ԈȊO�̃G�N�X�|�[�g
		export_results();

		//�I������
		terminate();
	}
	
}
