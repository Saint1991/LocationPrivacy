#ifdef SIMULATORCOMPONENTS_EXPORTS
#define BASIC_POI_API __declspec(dllexport)
#else
#define BASIC_POI_API __declspec(dllimport)
#endif

#pragma once
#include "Node.h"
#include "BasicRoad.h"
#include "PoiData.h"

namespace Map
{

	///<summary>
	/// POIを表すクラス
	/// connect_toはオーバーライドして何もしないメソッドにしているので使用しないこと
	/// NodeCollectionFactoryでもConnect系のヘルパメソッドは意味がないので使えない
	///</summary>
	class BASIC_POI_API BasicPoi : public Graph::Node<Geography::PoiData, BasicRoad>
	{
	private:
		
	public:
		
		BasicPoi(Graph::node_id id, Geography::PoiData data);
		BasicPoi(Graph::node_id id, Geography::LatLng position, std::string venue_name, std::string category_id, std::string category_name);
		BasicPoi(const BasicPoi& poi);
		~BasicPoi();

		bool connect_to(std::shared_ptr<BasicRoad> road);

		const std::string category_id() const;
		const std::string category_name() const;
		const std::string name() const;
		double lat() const;
		double lng() const;

		bool set_relation(Graph::node_id id1, double distance_to1, Graph::node_id id2, double distance_to2);
		bool set_relation(std::pair<Graph::node_id, double> relation1, std::pair<Graph::node_id, double> relation2);

		std::pair<Graph::node_id, Graph::node_id> between() const;
	};
}


