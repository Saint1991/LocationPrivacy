#pragma once
#include "stdafx.h"
#include "BaseEvaluationModule.h"
#include "LatLng.h"
#include "Coordinate.h"
#include "GeoCalculation.h"
#include "GraphUtility.h"

namespace Evaluation
{
	template <typename ENTITY_MANAGER, typename REQUIREMENT, typename POSITION_TYPE = Geography::LatLng>
	class ARSizeCalculationModule : public BaseEvaluationModule<ENTITY_MANAGER, REQUIREMENT>
	{
	public:
		static const metric_name AR_SIZE;

		ARSizeCalculationModule(std::shared_ptr<ENTITY_MANAGER> entities, std::shared_ptr<REQUIREMENT> requirement, std::shared_ptr<Time::TimeSlotManager> time_manager);
		~ARSizeCalculationModule();
		std::pair<metric_name, metric_value> calculate_evaluation_metric() const;
	};

	template <typename ENTITY_MANAGER, typename REQUIREMENT>
	class ARSizeCalculationModule<ENTITY_MANAGER, REQUIREMENT, Graph::Coordinate>
	{
		std::pair<metric_name, metric_value> calculate_evaluation_metric() const;
	};

}

#include "ARSizeCalculationModule.hpp"
