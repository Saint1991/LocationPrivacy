#pragma once
#include "stdafx.h"
#include "BasicGeoNodeData.h"
#include "BasicEdge.h"
#include "Identifiable.h"

namespace Geography
{
	
	typedef std::string poi_id;
	typedef std::pair<std::pair<Graph::node_id, Graph::node_id>, std::pair<double, double>> node_poi_relation;

	///<summary>
	/// POI��\���e���v���[�g�N���X
	/// POI_DATA�ɂ�BasicGeoNodeData���p������N���X���g����
	/// connection_map�Ńm�[�h�Ƃǂ̂悤�ɐڑ����Ă��邩��ێ�����
	///</summary>
	template <typename POI_DATA>
	class Poi : Identifiable<poi_id>
	{
	
	static_assert(std::is_base_of<BasicGeoNodeData, POI_DATA>::value, "template type POI_DATA is not derived from BasicGeoNodeData");
	

	protected:
		std::unique_ptr<std::unordered_map<node_poi_relation>> connection_map;
		std::shared_ptr<POI_DATA> data;

	public:
		Poi(poi_id id, std::shared_ptr<POI_DATA> data);
		virtual ~Poi();

		std::shared_ptr<POI_DATA const> get_static_data() const;
		std::shared_ptr<POI_DATA> get_data();

		void add_relation(const node_poi_relation& relation);
		std::list<Graph::node_id> get_srounding_nodes() const;

	};
}

#include "Poi.hpp"


