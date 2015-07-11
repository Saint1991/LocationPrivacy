#include "stdafx.h"
#include "CppUnitTest.h"
#include "Node.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(NodeTest)
	{
	public:
		const double precision = 1.0e-10;

		TEST_METHOD(constructor)
		{
			Graph::node_id expected = 1;
			Graph::Node<double, double> node(expected, 0.8);
			Graph::node_id actual = node.get_id();
			Assert::AreEqual(expected, actual);
			Assert::AreEqual(0.8, *node.data);
		}

		TEST_METHOD(connect_to_is_connecting_to)
		{
			Graph::Node<double, double> node(1, 0.8);
			Assert::IsTrue(node.connect_to((Graph::node_id)2, 0.3));
			Assert::IsTrue(node.connect_to((Graph::node_id)3, 1.0));
			Assert::IsFalse(node.connect_to(2, 0.5));

			Assert::IsFalse(node.is_connecting_to(1));
			Assert::IsTrue(node.is_connecting_to(2));
			Assert::IsTrue(node.is_connecting_to(3));
			Assert::IsFalse(node.is_connecting_to(4));
		}

		TEST_METHOD(disconnect_from)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to((Graph::node_id)2, 0.3);
			node.connect_to((Graph::node_id)3, 1.0);
			
			Assert::IsTrue(node.disconnect_from((Graph::node_id)2));
			Assert::IsFalse(node.disconnect_from((Graph::node_id)2));
			Assert::IsFalse(node.disconnect_from((Graph::node_id)1));

			Assert::IsFalse(node.is_connecting_to(1));
			Assert::IsFalse(node.is_connecting_to(2));
			Assert::IsTrue(node.is_connecting_to(3));
			Assert::IsFalse(node.is_connecting_to(4));
		}

		TEST_METHOD(get_edge_data)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to((Graph::node_id)2, 0.3);
			node.connect_to((Graph::node_id)3, 1.0);
			
			Assert::AreEqual(0.3, *node.get_edge_data((Graph::node_id)2), precision);
			Assert::AreEqual(1.0, *node.get_edge_data((Graph::node_id)3), precision);
			Assert::IsTrue(node.get_edge_data((Graph::node_id)4) == nullptr);
		}

		TEST_METHOD(connect_to2)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to((Graph::node_id)2, 0.3);
			node.connect_to((Graph::node_id)3, 1.0);

			Assert::AreEqual(0.3, *node.get_edge_data((Graph::node_id)2), precision);
			Assert::AreEqual(1.0, *node.get_edge_data((Graph::node_id)3), precision);

			Assert::IsFalse(node.connect_to(2, 0.5));
			Assert::AreEqual(0.3, *node.get_edge_data((Graph::node_id)2), precision);
		}
		
		TEST_METHOD(update_edge_data)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to((Graph::node_id)2, 0.3);
			node.connect_to((Graph::node_id)3, 1.0);

			Assert::IsTrue(node.update_edge_data((Graph::node_id)2, 0.8));
			Assert::IsFalse(node.update_edge_data((Graph::node_id)1, 0.4));
						
			Assert::AreEqual(0.8, *node.get_edge_data((Graph::node_id)2), precision);
			Assert::AreEqual(1.0, *node.get_edge_data((Graph::node_id)3), precision);
			Assert::IsFalse(node.is_connecting_to(1));
		}

		TEST_METHOD(connect_or_update)
		{
			Graph::Node<double, double> node(1, 0.8);
			Assert::IsTrue(node.connect_or_update(2UL, 0.3));
			Assert::IsTrue(node.connect_or_update(3UL, 1.0));
			
			Assert::AreEqual(0.3, *node.get_edge_data(2UL), precision);
			Assert::AreEqual(1.0, *node.get_edge_data(3UL), precision);

			Assert::IsFalse(node.connect_or_update(2UL, 0.8));
			Assert::AreEqual(0.8, *node.get_edge_data(2UL), precision);
		}

		TEST_METHOD(get_connecting_node_list)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to(2UL, 0.3);
			node.connect_to(3UL, 1.0);
			node.connect_to(4UL, 0.2);

			Graph::Node<double, double> node2 = node;
			std::list<Graph::node_id> connecting_list =node2.get_connecting_node_list();
			
			auto iter = connecting_list.begin();
			Assert::AreEqual(2UL, *iter);
			Assert::AreEqual(3UL, *++iter);
			Assert::AreEqual(4UL, *++iter);
		}

		TEST_METHOD(cp_constructor1)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to(2UL, 0.3);
			node.connect_to(4UL, 0.2);
			node.connect_to(3UL, 1.0);
			
			Graph::Node<double, double> node2 = node;
			Assert::AreEqual(0.8, *node.data, precision);
			Assert::IsFalse(node2.is_connecting_to(1UL));
			Assert::IsTrue(node2.is_connecting_to(2UL));
			Assert::IsTrue(node2.is_connecting_to(3UL));
			Assert::IsTrue(node2.is_connecting_to(4UL));
			Assert::AreEqual(0.3, *node2.get_edge_data(2UL), precision);
			Assert::AreEqual(1.0, *node2.get_edge_data(3UL), precision);
			Assert::AreEqual(0.2, *node2.get_edge_data(4UL), precision);
		}

		TEST_METHOD(cp_constructor2)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to(2UL, 0.3);
			node.connect_to(4UL, 0.2);
			node.connect_to(3UL, 1.0);

			Graph::Node<double, double> node2 = node;
			node2.update_edge_data(2UL, 0.8);
			Assert::AreEqual(0.8, *node2.get_edge_data(2UL));
			Assert::AreEqual(0.3, *node.get_edge_data(2UL));
		}

		TEST_METHOD(operator_bl_eq1)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to(2UL, 0.3);
			node.connect_to(4UL, 0.2);
			node.connect_to(3UL, 1.0);

			Graph::Node<double, double> node2(1, 0.4);
			node.connect_to(2UL, 0.3);
			node.connect_to(4UL, 0.2);
			
			Assert::IsTrue(node == node2);
		}

		TEST_METHOD(operator_bl_eq2) 
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to(2UL, 0.3);
			node.connect_to(4UL, 0.2);

			Graph::Node<double, double> node2(3, 0.8);
			node.connect_to(2UL, 0.3);
			node.connect_to(4UL, 0.2);

			Assert::IsFalse(node == node2);
		}

		TEST_METHOD(operator_bl_neq1)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to(2UL, 0.3);
			node.connect_to(4UL, 0.2);

			Graph::Node<double, double> node2(3, 0.8);
			node.connect_to(2UL, 0.3);
			node.connect_to(4UL, 0.2);

			Assert::IsTrue(node != node2);
		}

		TEST_METHOD(operator_bl_neq2)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to(2UL, 0.3);
			node.connect_to(4UL, 0.2);
			node.connect_to(3UL, 1.0);

			Graph::Node<double, double> node2(1, 0.4);
			node.connect_to(2UL, 0.3);
			node.connect_to(4UL, 0.2);

			Assert::IsFalse(node != node2);
		}
	};
}