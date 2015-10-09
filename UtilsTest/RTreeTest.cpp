#include "stdafx.h"
#include "CppUnitTest.h"
#include "boost\geometry\index\rtree.hpp"
#include "Node.h"
#include "LatLng.h"
#include "Edge.h"
#include "PoiData.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace geometry = boost::geometry;
namespace index = boost::geometry::index;
namespace model = boost::geometry::model;

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;


namespace RtreeTest
{
	TEST_CLASS(RTreeTest)
	{
		typedef bg::model::point<float, 2, bg::cs::cartesian> point;
		typedef bg::model::box<point> box;
		typedef std::pair<box, unsigned> value;

	public:

		TEST_METHOD(RTreeSample1)
		{
			bgi::rtree< value, bgi::quadratic<16> > rtree;
			for (unsigned i = 0; i < 10; ++i)
			{
				// create a box
				box b(point(i + 0.0f, i + 0.0f), point(i + 0.5f, i + 0.5f));
				// insert new value
				rtree.insert(std::make_pair(b, i));
			}

			box query_box(point(0, 0), point(5, 5));
			std::vector<value> result_s;
			rtree.query(bgi::intersects(query_box), std::back_inserter(result_s));
			rtree.query(bgi::contains(query_box), std::back_inserter(result_s));
		}

		typedef std::shared_ptr<Graph::Node<Geography::PoiData, Graph::Edge<double>>> m_value;
		class m_indexable_getter
		{
		public:
			typedef point result_type;
			result_type operator()(m_value val) const
			{
				double x = val->data->get_position().x();
				double y = val->data->get_position().y();
				return point(x, y);
			}
		};

		TEST_METHOD(RTreeSample2)
		{
			bgi::rtree<m_value, bgi::quadratic<16>, m_indexable_getter> tree;
			m_value val = std::make_shared<Graph::Node<Geography::PoiData, Graph::Edge<double>>>(1, std::make_shared<Geography::PoiData>("a", "b", "c", 0.5, 0.5));
			tree.insert(val);
			box query_box(point(0, 0), point(5, 5));
			std::vector<m_value> result_s;
			tree.query(bgi::within(query_box), std::back_inserter(result_s));
		}
	};
}
