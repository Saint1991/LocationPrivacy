#pragma once
#include "BasicDbMap.h"
#include "LatLng.h"
#include "BasicUser.h"
#include "Dummy.h"
#include "BasicRequirement.h"
#include "IProposedMethod.h"
#include "Rectangle.h"
#include "PauseMobileEntity.h"
#include "KatoMethodRequirement.h"
#include "Probability.h"

namespace Method
{

	///<summary>
	/// ���������Ď�@�̎���
	///</summary>
	class KatoMethod_UserChange
		: public Framework::IProposedMethod<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement>
	{
	private:
		//�ϐ�
		const int CELL_NUM_ON_SIDE = 3;

		typedef std::vector<Graph::Rectangle<Geography::LatLng>> Grid;
		std::vector<Grid> grid_list;

		typedef unsigned int entity_id;

		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> creating_dummy;

		std::vector<Graph::Rectangle<Geography::LatLng>> make_grid(double grid_length, const Geography::LatLng& center, int cell_num_on_side);//Grid�̍쐬
		//std::vector<std::vector<int>> make_table_of_entity_num_in_cell_at_phase(std::vector<Graph::Rectangle<Geography::LatLng>> grid_list, int phase);
		std::vector<int> get_total_num_of_each_cell(std::vector<std::vector<int>>& entities_table);//�e�Z���̃t�F�[�Y�g�[�^���̃G���e�B�e�B�̐�
		void decide_base_positions_and_arrive_time(int dummy_id);// �������_�~�[(k�Ԗ�)�̊�n�_����ъ�n�_�������Ԃ̌���
		void decide_share_positions_and_arrive_time(int dummy_id);// �������_�~�[(k�Ԗ�)�̋��L�n�_����ы��L�n�_�������Ԃ̌���
		void decide_destination_on_the_way(int dummy_id);// �������_�~�[(k�Ԗ�)�̈ړ��o�H�̌���
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
		KatoMethod_UserChange(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~KatoMethod_UserChange();

		void run();

	};
}


