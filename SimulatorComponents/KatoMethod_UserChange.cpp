#include "stdafx.h"
#include "KatoMethod_UserChange.h"

namespace Method
{
	
	///<summary>
	/// �R���X�g���N�^
	/// �����Simulator�ō쐬�����e����͂ւ̎Q�Ƃ�n��
	///</summary>
	KatoMethod_UserChange::KatoMethod_UserChange(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement>(map, user, requirement, time_manager),grid_list(std::vector<Grid>(time_manager->phase_count()))
	{
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


	/*
	///<summary>
	/// T[s]���Ƃ̃O���b�h�̈���쐬
	/// grid_length�̓O���b�h�S�̂̒���
	///</summary>
	std::vector<std::vector<int>> KatoMethod_UserChange::make_table_of_entity_num_in_cell_at_phase(std::vector<Graph::Rectangle> grid_list, int phase)
	{

	}
	*/

	
	///<summary>
	/// �O���b�h�e�[�u���̊e�O���b�h���Ƃ̃G���e�B�e�B�̍��v���擾
	///</summary>
	std::vector<int> KatoMethod_UserChange::get_total_num_of_each_cell(std::vector<std::vector<int>>& entities_table) {
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
	/// �������_�~�[(k�Ԗ�)�̊�n�_����ъ�n�_�������Ԃ̌���
	///</summary>
	void KatoMethod_UserChange::decide_base_positions_and_arrive_time(int dummy_id)
	{
		int phase = 0;//phase
		const int GRID_TOTAL_NUM = CELL_NUM_ON_SIDE*CELL_NUM_ON_SIDE;//�O���b�h�̐�
		
		//�e�O���b�h�̊e�t�F�C�Y�ɂ�����entities�̐����L�����邽�߂�table(���I�z��)�̍쐬
		std::vector<std::vector<int>> entities_num_table(GRID_TOTAL_NUM, std::vector<int>(time_manager->phase_count(),0));
		

		while (phase <= time_manager->phase_count())
		{
			std::shared_ptr<Geography::LatLng const> center = entities->get_average_position_of_phase(phase);//���S�ʒu�����߂�
			Grid grid = make_grid(requirement->required_anonymous_area, *center, CELL_NUM_ON_SIDE);//phase���ƂɃO���b�h���쐬
			grid_list.at(phase) = grid;

			//����phase�ɂ�����e�Z���ɑ��݂��郆�[�U�y�ѐ����ς݃_�~�[�̈ړ��o�H(��~�n�_)�̐�
			//�����Z����id�C�c��phase��\�����I�Q�����z��ŋL��
			//k-2�ڂ܂ł�table������Ă����āCk-1�ڂ��{�P���čX�V����΂��������ǂ�
			int cell_id = 0;//�Z����id
			for (std::vector<Graph::Rectangle<Geography::LatLng>>::iterator iter = grid.begin(); iter != grid.end(); iter++)
			{
				entities_num_table.at((cell_id++) - 1).at(phase) = entities->get_entity_count_within_boundary(phase, *iter);
			}

			phase++;//�T�[�r�X���p�Ԋu��time_manager���璀�ꋁ�߂Ȃ��Ƃ����Ȃ�����
		}

		//�S�Ẵt�F�[�Y�ɂ�����e�Z���̃G���e�B�e�B�̍��v
		std::vector<int> total_entity_num_all_phase = get_total_num_of_each_cell(entities_num_table);
		
				
		//�S�Ẵt�F�[�Y�ɂ����āC�G���e�B�e�B���ŏ��ɂȂ�Z��id���擾
		std::vector<int>::iterator cell_iter = std::min_element(total_entity_num_all_phase.begin(), total_entity_num_all_phase.end());
		size_t min_cell_id = std::distance(total_entity_num_all_phase.begin(), cell_iter);

		//min_cell_id�̃Z���ōŏ��ɂȂ�ŏ���phase���擾
		std::vector<int>::iterator phase_iter = std::min_element(entities_num_table.at(min_cell_id).begin(), entities_num_table.at(min_cell_id).end());
		int base_phase = std::distance(entities_num_table.at(min_cell_id).begin(), phase_iter);

		
		//�擾����cell_id,phase�ɂ������~�n�_���擾
		//��l���z�Ń����_���擾
		//poi���Ȃ��������̏ꍇ�������l�����K�v��������Ȃ�
		Graph::Rectangle<Geography::LatLng> cell = grid_list.at(base_phase).at(min_cell_id);
		std::vector<std::shared_ptr<Map::BasicPoi const>> poi_within_base_point_grid = map->find_pois_within_boundary(cell);
		Math::Probability generator;
		int index = generator.uniform_distribution(0, poi_within_base_point_grid.size() - 1);
		std::shared_ptr<Map::BasicPoi const> poi = poi_within_base_point_grid.at(index);
		
		Graph::node_id base_poi_id = poi->get_id();
		Geography::LatLng base_point = poi->data->get_position();
		
		entities->get_dummy_by_id(dummy_id)->set_position_of_phase(base_phase, Graph::MapNodeIndicator(base_poi_id),base_point);
		
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
				if (entities->read_dummy_by_id(dummy_id)->read_position_of_phase(time_manager->phase_count()) <= share_phase)
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_info = entities->read_dummy_by_id(dummy_id)->find_previous_fixed_position(share_phase);
					time_t previous_time_limit = time_manager->find_phase_of_time(share_phase) - time_manager->find_phase_of_time(previous_info.first) - entities->read_dummy_by_id(dummy_id)->get_pause_time(previous_info.first);
					//���L�ꏊ�ɓ��B�\�Ȃ炻�̈ʒu��ݒ肵�C���B�s�\�Ȃ�΂�����x�ʂ̃t�F�[�Y������
					if (map->is_reachable(previous_info.second.first, share_position.first, requirement->average_speed_of_dummy, previous_time_limit)) {
						break;
					}else {
						goto ONCE_AGAIN;
					}
				}
				//�������_�~�[�̊��ɒ�~�ʒu�����肵�Ă���t�F�[�Y�������L�t�F�[�Y���������ꍇ
				else if (entities->read_dummy_by_id(dummy_id)->find_next_fixed_position(0).first >= share_phase)
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> next_info = entities->read_dummy_by_id(dummy_id)->find_next_fixed_position(0);
					time_t next_time_limit = time_manager->find_phase_of_time(next_info.first) - time_manager->find_phase_of_time(share_phase) - entities->read_dummy_by_id(dummy_id)->get_pause_time(share_phase);
					if (map->is_reachable(share_position.first, next_info.second.first, requirement->average_speed_of_dummy, next_time_limit)) {
						break;
					}
					else {
						goto ONCE_AGAIN;
					}
				}
				//�������_�~�[�̊��ɒ�~�ʒu�����肵�Ă���t�F�[�Y�̊Ԃɂ���ꍇ
				else
				{
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> previous_info = entities->read_dummy_by_id(dummy_id)->find_previous_fixed_position(share_phase);
					std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> next_info = entities->read_dummy_by_id(dummy_id)->find_next_fixed_position(share_phase);
					//time_limit��previous�ł̒�~���Ԃ��l�����Ȃ���΂Ȃ�Ȃ����Ƃɒ���
					time_t previous_time_limit = time_manager->find_phase_of_time(share_phase) - time_manager->find_phase_of_time(previous_info.first) - entities->read_dummy_by_id(dummy_id)->get_pause_time(previous_info.first);
					time_t next_time_limit = time_manager->find_phase_of_time(next_info.first) - time_manager->find_phase_of_time(share_phase) -  entities->read_dummy_by_id(dummy_id)->get_pause_time(share_phase);

					if (map->is_reachable(previous_info.second.first, share_position.first, requirement->average_speed_of_dummy, previous_time_limit)
						&& map->is_reachable(share_position.first, next_info.second.first, requirement->average_speed_of_dummy, next_time_limit)) {
						break;
					}
					else {
						goto ONCE_AGAIN;
					}
				}

				//Dmincross = += 1;
				//�������̃_�~�[�̌����� += 1;
				entities->get_dummy_by_id(dummy_id)->set_crossing_position_of_phase(share_phase, share_position.first, *share_position.second);
				int target_dummy_cross_count = target->get_cross_count();
				target_dummy_cross_count++;//���̂�����target�̌����񐔂̍��v�𐔂�����H
				break;
					
				ONCE_AGAIN:
					continue;
			}
			//���[�U�y�ѐ����ς݃_�~�[�̕��ό����񐔂����C�������_�~�[�̌����񐔂������Ȃ�܂ŋ��L�n�_��ݒ肷��
			if(entities->get_dummy_by_id(dummy_id)->get_cross_count() > entities->get_all_entities_total_crossing_count() / dummy_id) break;
		}
	}


	/*
	///<summary>
	/// �������_�~�[(k�Ԗ�)�̈ړ��o�H�̌���
	///</summary>
	void KatoMethod_UserChange::decide_destination_on_the_way(int dummy_id)
	{
		//�������_�~�[�̊��Ɍ��܂��Ă��钆�ōŏ��̒n�_���擾
		int dest_phase = entities->get_dummy_by_id(dummy_id)->find_next_fixed_position(0).first;
		std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> dest_position = entities->get_dummy_by_id(dummy_id)->find_next_fixed_position(0);

		//------------------------------------�������ʒu�̌��聫------------------------------------------------------//
		time_t init_pause_time = 0;
		time_t init_arrive_time = time_manager->time_of_phase(0);
		
		
		//�������_�~�[�̃v�����̒��ŁC��ԍŏ��̏ꏊ����0�b�܂ł͈̔�(�ő��~���Ԃ��l��)�œ����ł���POI���擾
		//��U���X�g�Ŏ擾���Ă���C���̒����烉���_���őI�ԕ����ǂ�
		
		std::pair<int, std::pair<Graph::MapNodeIndicator, std::shared_ptr<Geography::LatLng const>>> init_position = entities->get_dummy_by_id(dummy_id)->find_next_fixed_position(0);// getpauseposition;
		
		
		
		//PPout��<position, start, pause��init>��ǉ�;
		entities->get_dummy_by_id(dummy_id)->set_position_of_phase(0, init_position.second.first, *init_position.second.second);

		//------------------------------------�������ʒu�̌��聪------------------------------------------------------//
		

		//�����ʒu�ȍ~�̒�~�n�_�̌���
		int phase_id = 1;
		while (phase_id <= time_manager->phase_count())
		{
			//�A���Œ�~�ʒu�����܂��Ă���ꍇ��skip
			if (entities->get_dummy_by_id(dummy_id)->find_next_fixed_position((phase_id)-1).first == phase_id) { break; }
			else {
				//position(phase_id-1)��position(position_id)�ɓ��B�\�Ȃ�΁Cphase�ɂ�intersection��ǉ����C�����łȂ��Ȃ�r����~�ʒu��ݒ�
				time_t time_limit = 0;
				//intersection��ǉ�
				if (map->is_reachable(entities->get_dummy_by_id(dummy_id)->find_previous_fixed_position(dest_position.first).second.first, dest_position.second.first, requirement->average_speed_of_dummy, requirement->max_pause_time)) {
					//map����ŒZ�H��path������Ă���I�I
				}
				//�r����~��ݒ�
				else {
					//phase_id�Ԗڂ̒�~���Ԃ�����
					time_t pause_time = std::rand() % (requirement->min_pause_time + requirement->max_pause_time + 1.0);
					entities->get_dummy_by_id(dummy_id)->set_pause_time(phase_id, pause_time);

					//positioni-1����positioni�Ԗڂ֓��B�\��POI����ЂƂ����_���Ŏ擾
					Geography::LatLng position = get_pause_position;

					entities->get_dummy_by_id(dummy_id)->set_position_of_phase(phase_id, position);


				}
			}

			//�r���ړI�n���~�n�_�Ƃ��Č���
			

		}




	}*/

	///<summary>
	/// �_�~�[�̍s���v�������C������
	///</summary>
	void KatoMethod_UserChange::revise_dummy_movement_plan()
	{

	}


	///<summary>
	/// �_�~�[�̒�~���Ԃ̏C��
	///</summary>
	void KatoMethod_UserChange::revise_dummy_pause_time()
	{

	}


	///<summary>
	/// �_�~�[�̈ړ��o�H�̏C��
	///</summary>
	void KatoMethod_UserChange::revise_dummy_trajectory()
	{

	}


	///<summary>
	/// �_�~�[�̍s�����x�̏C��
	///</summary>
	void KatoMethod_UserChange::revise_dummy_speed()
	{

	}


	///<summary>
	/// �_�~�[�̒�~�ʒu�̏C��
	///</summary>
	void KatoMethod_UserChange::revise_dummy_pose_position()
	{

	}


	///<summary>
	/// ���[�U�̍s���v�����ύX�̔��f
	///</summary>
	void KatoMethod_UserChange::check_user_plan()
	{

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
		/*
		for (size_t dummy_id = 1; dummy_id <= entities->get_dummy_count(); dummy_id++)
		{
			//decide_base_positions_and_arrive_time(dummy_id);// �������_�~�[(k�Ԗ�)�̊�n�_����ъ�n�_�������Ԃ̌���
			//decide_share_positions_and_arrive_time(dummy_id);// �������_�~�[(k�Ԗ�)�̋��L�n�_����ы��L�n�_�������Ԃ̌���
			//decide_dummy_path(dummy_id);// �������_�~�[(k�Ԗ�)�̈ړ��o�H�̌���
			
		}*/
	}
	
	
	///<summary>
	/// ��������Ď�@�̊j�ɂȂ镔��.�_�~�[�̍s�����C������
	///</summary>
	void KatoMethod_UserChange::revise_dummy_positions()
	{

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

		//�����Ń_�~�[�̍s�����C������
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
