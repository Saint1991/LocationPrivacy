#pragma once
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "IProposedMethod.h"
#include "Rectangle.h"

namespace Method
{

	///<summary>
	/// ���������Ď�@�̎���
	///</summary>
	class KatoMethod_UserChange
		: public Framework::IProposedMethod<Map::BasicDbMap, User::BasicUser<Geography::LatLng>, Entity::Dummy<Geography::LatLng>, Requirement::BasicRequirement>
	{
	private:
		//�ϐ�
		int cell_nu_on_side;

		std::vector<Graph::Rectangle> make_grid(double grid_length, const Geography::LatLng& center, int cell_num_on_side);//Grid�̍쐬
		std::vector<std::vector<int>> make_table_of_entity_num_in_cell_at_phase(std::vector<Graph::Rectangle> grid_list, int phase);
		std::vector<int> get_total_num_of_each_cell(std::vector<std::vector<int>>& entities_table);//�e�Z���̃t�F�[�Y�g�[�^���̃G���e�B�e�B�̐�
		Graph::Rectangle get_min_dummy_cell();//���[�U����у_�~�[�����݂��鐔���ŏ��̃Z�����擾
		time_t get_min_dummy_num_time();//T�b�Ԃ̃��[�U�y�у_�~�[�����݂��鐔���ŏ��ƂȂ�ŏ��̎������擾
		void decide_base_positions_and_arrive_time(int dummy_id);// �������_�~�[(k�Ԗ�)�̊�n�_����ъ�n�_�������Ԃ̌���
		void decide_share_positions_and_arrive_time(int dummy_id);// �������_�~�[(k�Ԗ�)�̋��L�n�_����ы��L�n�_�������Ԃ̌���
		void decide_dummy_path(int dummy_id);// �������_�~�[(k�Ԗ�)�̈ړ��o�H�̌���
		void revise_dummy_movement_plan();//�_�~�[�̍s���v�������C������
		void revise_dummy_pause_time();//�_�~�[�̒�~���Ԃ̏C��
		void revise_dummy_trajectory();//�_�~�[�̈ړ��o�H�̏C��
		void revise_dummy_speed();//�_�~�[�̍s�����x�̏C��
		void revise_dummy_pose_position();//�_�~�[�̒�~�n�_�̏C��
		void check_user_plan();//���[�U�̍s���v�����ύX�̃`�F�b�N

	protected:
		void initialize();
		void decide_dummy_positions();
		void revise_dummy_positions();
		void evaluate();
		void export_results();
		void terminate();

		
	public:
		KatoMethod_UserChange(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<User::BasicUser<Geography::LatLng>> user, std::shared_ptr<Requirement::BasicRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~KatoMethod_UserChange();

		void run();

	};
}


