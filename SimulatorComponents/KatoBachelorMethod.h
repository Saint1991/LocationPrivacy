#ifdef SIMULATORCOMPONENTS_EXPORTS
#define KATO_BACHELOR_METHOD_API __declspec(dllexport)
#else
#define KATO_BACHELOR_METHOD_API __declspec(dllimport)
#endif
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
#include "GeoCalculation.h"

namespace Method
{

	///<summary>
	/// ��������w�����Ƙ_���̒�Ď�@�̎���
	///</summary>
	class KATO_BACHELOR_METHOD_API KatoBachelorMethod
		: public Framework::IProposedMethod<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement>
	{
	private:
		//�ϐ�
		const int CELL_NUM_ON_SIDE = 3;//�O���b�h�̃Z���̈�ӂ̐�

		typedef std::vector<Graph::Rectangle<Geography::LatLng>> Grid;
		std::vector<Grid> grid_list;

		typedef unsigned int entity_id;

		std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> creating_dummy;
		
		//���\�b�h
		std::vector<Graph::Rectangle<Geography::LatLng>> make_grid(double grid_length, const Geography::LatLng& center, int cell_num_on_side);//Grid�̍쐬
		std::vector<int> get_total_num_of_each_cell_at_interval_phase(std::vector<std::vector<int>>& entities_table, int start_phase, int end_phase);//�e�Z���̃t�F�[�Y�g�[�^���̃G���e�B�e�B�̐�
		std::vector<int> get_total_num_of_each_cell_at_all_phase(std::vector<std::vector<int>>& entities_table);//�e�Z���̃t�F�[�Y�g�[�^���̃G���e�B�e�B�̐�
		std::vector<std::shared_ptr<Map::BasicPoi const>> candidate_pois_list(const Graph::Rectangle<Geography::LatLng>& boundary);
		void linear_interpolation_of_path_between_positions(const Graph::MapNodeIndicator& source, const Graph::MapNodeIndicator& destination, int *phase_id, int *dest_rest_time);
		void decide_base_positions_and_arrive_time(int dummy_id);// �������_�~�[(k�Ԗ�)�̊�n�_����ъ�n�_�������Ԃ̌���
		void decide_share_positions_and_arrive_time(int dummy_id);// �������_�~�[(k�Ԗ�)�̋��L�n�_����ы��L�n�_�������Ԃ̌���
		void decide_destination_on_the_way(int dummy_id);// �������_�~�[(k�Ԗ�)�̈ړ��o�H�̌���
				
	protected:
		
		void initialize();
		void decide_dummy_positions();
		void evaluate();
		void export_results();
		void terminate();

		
	public:
		KatoBachelorMethod(std::shared_ptr<Map::BasicDbMap const> map, std::shared_ptr<Entity::PauseMobileEntity<Geography::LatLng>> user, std::shared_ptr<Requirement::KatoMethodRequirement const> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~KatoBachelorMethod();

		void run();

	};
}

