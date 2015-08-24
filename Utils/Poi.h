#ifdef UTILS_EXPORTS
#define POI_API __declspec(dllexport)
#else
#define POI_API __declspec(dllimport)
#endif

#pragma once
#include "stdafx.h"
#include "BasicGeoNodeData.h"
#include "BasicPath.h"

namespace Geography
{

	///<summary>
	/// POIを表すテンプレートクラス
	/// POI_DATAにはBasicGeoNodeDataを継承するクラスが使える
	/// connection_mapでノードとどのように接続しているかを保持する
	///</summary>
	template <typename POI_DATA>
	class POI_API Poi
	{
	
	static_assert(std::is_base_of<BasicGeoNodeData, POI_DATA>::value, "template type POI_DATA is not derived from BasicGeoNodeData");
	typedef std::pair<std::pair<Graph::node_id, Graph::node_id>, std::pair<double, double>> node_poi_relation;

	protected:
		std::unique_ptr<std::unordered_map<node_poi_relation>> connection_map;
		std::string id;
		std::shared_ptr<POI_DATA> data;

	public:
		Poi(std::string id, std::shared_ptr<POI_DATA> data);
		virtual ~Poi();

		const std::string get_id() const;
		std::shared_ptr<POI_DATA const> get_static_data() const;
		std::shared_ptr<POI_DATA> get_data();

		void add_relation(const node_poi_relation& relation);
		std::list<Graph::node_id> get_srounding_nodes() const;

	};
}

#include "Poi.hpp"


