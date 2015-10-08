#include "stdafx.h"
#include "KatoMethod_UserChange.h"

namespace Method
{
	
	///<summary>
	/// �R���X�g���N�^
	/// �����Simulator�ō쐬�����e����͂ւ̎Q�Ƃ�n��
	///</summary>
	KatoMethod_UserChange::KatoMethod_UserChange(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<User::BasicUser<Geography::LatLng>> user, std::shared_ptr<Requirement::BasicRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager)
		: Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement>(map, user, requirement, time_manager)
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
	std::vector<Graph::Rectangle> KatoMethod_UserChange::make_grid(double grid_length, const Geography::LatLng& center, int cell_num_on_side)
	{
		double side_length = grid_length / cell_num_on_side;//�Z������̒����`�̒���
		//center�̎l�_�̍��W
		double top = center.y() + side_length * 1.5;
		double left = center.x() - side_length * 1.5;
		double bottom = center.y() + side_length * 0.5;
		double right = center.x() - side_length * 0.5;
		 
		std::vector<Graph::Rectangle> grid_list;//�O���b�h�S�̂��Ǘ����郊�X�g
		
		double base_left = left;//����̐����`��left�����[�v�̍ۂ̊�Ƃ���
		double base_right = right;//����̐����`��right�����[�v�̍ۂ̊�Ƃ���

		for (int i = 0; i < cell_num_on_side; i++)
		{
			for (int j = 0; j < cell_num_on_side; j++)
			{
				grid_list.push_back(Graph::Rectangle(top, left, bottom, right));
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
	/// t�̎���D�̕��ψʒu�𒆐S�����߂�
	///</summary>
	Geography::LatLng KatoMethod_UserChange::get_center_position()
	{
		return Geography::LatLng(10.0, 10.0);
	}


	///<summary>
	/// �Z���Ɉʒu���郆�[�U�y�ѐ����ς݃_�~�[�̈ړ��o�HD={D_0,...,D_k-1}(��~�n�_�C��~�n�_��������)�̐�
	///</summary>
	int KatoMethod_UserChange::get_entities_path_num_in_cell(const std::vector<Graph::Rectangle>& grid_list) {
		int k=0;
		//entities->find_dummy_if();
		return k;
	}

	
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
	
	/*
	
	///<summary>
	/// �������_�~�[(k�Ԗ�)�̊�n�_����ъ�n�_�������Ԃ̌���
	///</summary>
	void KatoMethod_UserChange::decide_base_positions_and_arrive_time(int dummy_id)
	{
		
		time_t T = 1000;//�����F�����炭phase�Ŋ��邩�C�����Ŋ��邩���Ȃ��Ƃ����Ȃ�
		time_t t = time_manager->time_of_phase(0);

		while (t <= time_manager->time_of_phase(time_manager->phase_count()))
		{
			t += T;//�T�[�r�X���p�Ԋu��time_manager���璀�ꋁ�߂Ȃ��Ƃ����Ȃ�����
			
			//T[s]���ƂɃO���b�h���쐬
			Geography::LatLng center = get_center_position();
			std::vector<Graph::Rectangle> grid_list = make_grid(requirement->required_anonymous_area, center, 3);
			
			for (int i = 0; i < grid_list.size(); i++)
			{
				//�̈�G�ɑ��݂���_�~�[�̈ړ��o�H�̐�
				
				//grid_list�̊e�Z���͈͓̔��ɑ��݂���_�~�[�̐������߂�֐�������
				//

			}

		}

		Graph::Rectangle G_base = get_min_dummy_cell();//���[�U����у_�~�[�����݂��鐔���ŏ��̃Z��
		time_t t_base = get_min_dummy_num_time();//T�b�Ԃ̃��[�U����у_�~�[�̑��ݐ����ŏ��ƂȂ鎞��

		while (//p_base��G_base�ɑ���
			)
		{
			p_base = GetPausePosition();
		}

		//return entities->set_point_at();
	}

	
	

	///<summary>
	/// �������_�~�[(k�Ԗ�)�̋��L�n�_����ы��L�n�_�������Ԃ̌���
	///</summary>
	void KatoMethod_UserChange::decide_share_positions_and_arrive_time()
	{
		for (size_t i = 0; �������̃_�~�[�̌����� > ave(D0cross); i++)
		{
			D_min = std::min();
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
	void KatoMethod_UserChange::decide_dummy_path()
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
		//decide_base_positions_and_arrive_time();// �������_�~�[(k�Ԗ�)�̊�n�_����ъ�n�_�������Ԃ̌���
		//decide_share_positions_and_arrive_time();// �������_�~�[(k�Ԗ�)�̋��L�n�_����ы��L�n�_�������Ԃ̌���
		//decide_dummy_path();// �������_�~�[(k�Ԗ�)�̈ړ��o�H�̌���
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
