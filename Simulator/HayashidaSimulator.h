#ifdef SIMULATOR_EXPORTS
#define HAYASHIDA_SIMULATOR_API __declspec(dllexport)
#else
#define HAYASHIDA_SIMULATOR_API __declspec(dllimport)
#endif

#pragma once
#include "BasicDbMap.h"
#include "LatLng.h"
#include "PauseMobileEntity.h"
#include "KatoMethodRequirement.h"
#include "ISimulator.h"
#include "KatoMasterMethod.h"
#include "Rectangle.h"
#include "Probability.h"
#include "GeoCalculation.h"
#include "Trajectory.h"
#include "Dijkstra.h"
#include "FileExporter.h"

namespace Simulation
{
	///<summary>
	/// �V�~�����[�^����
	///</summary>
	class HAYASHIDA_SIMULATOR_API HayashidaSimulator 
		: public ISimulator<Map::BasicDbMap, Entity::PauseMobileEntity<Geography::LatLng>, Entity::PauseMobileEntity<Geography::LatLng>, Requirement::KatoMethodRequirement, Geography::LatLng, Graph::Trajectory<Geography::LatLng>>
	{
	private:
	#pragma region
		static constexpr auto USER_TRAJECTORY_OUT_PATH = "C:/Users/Mizuno/Desktop/user_trajectory";
		static constexpr double AVERAGE_SPEED = 1.5;
		static constexpr double RANGE_OF_SPEED = 0.5;
		static constexpr int MAX_PAUSE_TIME = 600;
		static constexpr int MIN_PAUSE_TIME = 60;
		static constexpr int SERVICE_INTERVAL = 360;
		static constexpr int end_time = 18000;
		static constexpr int POI_NUM = 2;
		static constexpr double base_lat = 35.677;//�o���n�̖ڈ��̈ܓx�D�SPOI�̕��ϒl
		static constexpr double base_lng = 139.715;//�o���n�̖ڈ��̌o�x�D�SPOI�̕��ϒl
		double length_of_rect = 0.005;//�����ɂ͓K�؂Ȕ͈͓��̈ܓx�o�x��������
	#pragma endregion �v���p�����[�^(�_�~�[��requirement�Ɠ����ɂ��邱��)
	
	protected:
		Graph::Rectangle<Geography::LatLng> map_boundary;
		void build_map(const Graph::Rectangle<Geography::LatLng>& boundary);
		void create_trajectories();
		void make_requirement_list();
		void random_user();

	public:
		HayashidaSimulator(const Graph::Rectangle<Geography::LatLng>& map_boundary);
		virtual ~HayashidaSimulator();
		void prepare();
		void run();
		void evaluate();
		void export_evaluation_result();
	};

}
