#pragma once
#include "NodeCollectionFactory.h"
#include "LatLng.h"
#include "IdentifiableCollection.h"
#include "RoutingMethod.h"
#include "RoutingTable.h"
#include "MapNodeIndicator.h"
#include "boost\geometry\index\rtree.hpp"
#include "Rectangle.h"

namespace Graph
{

	namespace geometry = boost::geometry;
	namespace model = geometry::model;
	namespace index = geometry::index;
	
	typedef model::point<double, 2, boost::geometry::cs::cartesian> point;
	typedef model::box<point> box;
	
	///<summary>
	/// �n�}�Ɋւ��钊�ۃN���X
	/// ��̓I�ȃ}�b�v�͂�����p�����č��
	/// NODE, PATH�ɂ͂��ꂼ��Node, Edge���p�������N���X���g����
	/// PATH�ɂ�BasicPathData���p�����Ă���f�[�^��ێ����Ă���Edge�����g���Ȃ��̂Œ���
	/// �Ƃ肠����R-Tree�̍��W�͈ܓx�C�o�x�ŌŒ肵�Ă���
	/// Map�̍\�z��load(Rectangle boundary)��ǂ񂾃^�C�~���O�Ŏn�܂�
	///</summary>
	template <typename NODE, typename POI, typename PATH>
	class Map
	{

		#pragma region RTree
		typedef std::shared_ptr<POI const> rtree_value;
		class MyPoiAdapter
		{
		public:
			typedef point result_type;
			point operator()(rtree_value val) const
			{
				double x = val->data->get_position().x();
				double y = val->data->get_position().y();
				return point(x, y);
			}
		};
		typedef index::rtree<rtree_value, index::quadratic<16>, MyPoiAdapter> rtree;
		#pragma endregion

	protected:
		std::unique_ptr<RoutingMethod<NODE, PATH>> routing_method;
		std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, NODE>> node_collection;
		std::shared_ptr<const Collection::IdentifiableCollection<Graph::node_id, POI>> poi_collection;
		std::unique_ptr<rtree> rtree_index;
		
		virtual void build_map(const Graph::Rectangle<Geography::LatLng>& boundary) = 0;
		void build_rtree_index();

		///<summary>
		/// �ŒZ�H���i�[���郋�[�e�B���O�e�[�u��
		/// Source����Destination�֍s���̂Ɏ��ɂǂ̃m�[�h�ɍs���K�v�����邩���i�[����D
		/// Map�̍\����initialize���Ăяo�����ۂɍs����
		///</summary>
		std::unique_ptr<RoutingTable const> routing_table;

	private:
		std::pair<node_id, node_id> get_intersection_ends_of_shortest_path(const MapNodeIndicator& from, const MapNodeIndicator& to) const;

	public:
		Map(std::unique_ptr<RoutingMethod<NODE, PATH>> routing_method);
		virtual ~Map();

		void load(const Graph::Rectangle<Geography::LatLng>& boundary);

		double shortest_distance(const MapNodeIndicator& from, const MapNodeIndicator& to) const;
		const std::vector<MapNodeIndicator> get_shortest_path(const MapNodeIndicator& source, const MapNodeIndicator& destination) const;
		double calc_necessary_time(const MapNodeIndicator& from, const MapNodeIndicator& to, const double& avg_speed) const;
		bool is_reachable(const MapNodeIndicator& from, const MapNodeIndicator& to, const double& avg_speed, const double& time_limit) const;
		
		virtual std::vector<std::shared_ptr<POI const>> find_pois_within_boundary(const box& boundary) const;
		virtual std::vector<std::shared_ptr<POI const>> find_pois_within_boundary(const Graph::Rectangle<>& boundary) const;
		virtual std::vector<std::shared_ptr<POI const>> find_pois_within_boundary(const Graph::Rectangle<Geography::LatLng>& boundary) const;

		std::shared_ptr<NODE const> get_static_node(const node_id& id) const;
		std::shared_ptr<POI const> get_static_poi(const node_id& poi_id) const;
	};
}

#include "Map.hpp"
