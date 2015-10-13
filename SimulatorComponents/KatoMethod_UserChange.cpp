#include "stdafx.h"
#include "KatoMethod_UserChange.h"

namespace Method
{
	
	///<summary>
	/// �R���X�g���N�^
	/// �����Simulator�ō쐬�����e����͂ւ̎Q�Ƃ�n��
	///</summary>
	KatoMethod_UserChange::KatoMethod_UserChange(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<User::BasicUser<Geography::LatLng>> user, std::shared_ptr<Requirement::BasicRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement>(map, user, requirement, time_manager),grid_list(std::vector<Grid>(time_manager->phase_count()))
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

	/*
	///<summary>
	/// ���[�U����у_�~�[�����݂��鐔���ŏ��̃Z�����擾
	///</summary>
	Graph::Rectangle KatoMethod_UserChange::get_min_dummy_cell()
	{
		return Graph::Rectangle(1.0,1.0,1.0,1.0);//�����ɕԂ��͍̂ŏ��̃Z��
	}

	
	///<summary>
	/// T�b�Ԃ̃��[�U�y�у_�~�[�����݂��鐔���ŏ��ƂȂ�ŏ��̎������擾
	///</summary>
	time_t KatoMethod_UserChange::get_min_dummy_num_time()
	{
		return 0;
	}
	*/
	
	
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
		//poi���Ȃ��������̏ꍇ�������l�����K�v��������Ȃ�
		Graph::Rectangle<Geography::LatLng> cell = grid_list.at(base_phase).at(min_cell_id);
		std::vector<std::shared_ptr<Map::BasicPoi const>> poi_within_base_point_grid = map->find_pois_within_boundary(cell);
		Math::Probability generator;
		int index = generator.uniform_distribution(0, poi_within_base_point_grid.size() - 1);
		std::shared_ptr<Map::BasicPoi const> poi = poi_within_base_point_grid.at(index);
		
		Geography::LatLng base_point = poi->data->get_position();
		
		entities->get_dummy_by_id(dummy_id)->set_position_of_phase(base_phase,base_point);
		
	}

	
	/*
	
	///<summary>
	/// �������_�~�[(k�Ԗ�)�̋��L�n�_����ы��L�n�_�������Ԃ̌���
	///</summary>
	void KatoMethod_UserChange::decide_share_positions_and_arrive_time(int dummy_id)
	{
		while (�������̃_�~�[�̌����� > ave(D0cross))
		{
			unsigned int min_cross_entity_id = entities->get_min_cross_entity_id();
			
			//user��dummy�ŏꍇ����
			Geography::LatLng share_position = 
			p_shared = random(Dmincross�̒�~�n�_);
			t_shared = p_shared�ɓ������鎞��;

			if (PPin�Ɋ܂܂��S�Ă̒�~�n�_��������<=t_shared)
			{
				while (���B�\���ǂ���) {
					t_last = std::max(PPin�Ɋ܂܂���~�n�_��������);
					p_last = PPin�Ɋ܂܂��t_last�ɓ��������~�n�_;
				}
				
			}
			else if (PPin�Ɋ܂܂��S�Ă̒�~�n�_�������� >= t_shared)
			{
				while (���B�\���ǂ���) {
					t_first = std::min(PPin�Ɋ܂܂���~�n�_��������);
					p_first = PPin�Ɋ܂܂��t_first�ɓ��������~�n�_;
				}
			}
			else
			{
				while (previous��shared�̓��B�\���ǂ���) {
					t_previous = a;
				}
			}

			PP��<p_shared, t_shared>��ǉ�
			Dmincross = += 1;
			�������̃_�~�[�̌����� += 1;
		}
		
	
	}

	///<summary>
	/// �������_�~�[(k�Ԗ�)�̈ړ��o�H�̌���
	///</summary>
	void KatoMethod_UserChange::decide_dummy_path(int dummy_id)
	{

	}

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
	*/
	

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
