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

		TEST_METHOD(connect_to1_and_get_edge_to)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to(Graph::Edge<double>((Graph::node_id)3, 1.0));
			node.connect_to(Graph::Edge<double>((Graph::node_id)2, 0.3));
			
			std::shared_ptr<Graph::Edge<double>> ret1 = node.get_edge_to(3);
			auto ret2 = node.get_edge_to(4);

			Assert::AreEqual((Graph::node_id)3, ret1->to);
			Assert::AreEqual(1.0, *(ret1->data));
			Assert::IsTrue(ret2 == nullptr);
		}

		TEST_METHOD(connect_to2_and_get_edge_to)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to((Graph::node_id)3, 1.0);
			node.connect_to((Graph::node_id)2, 0.3);

			std::shared_ptr<Graph::Edge<double>> ret1 = node.get_edge_to(3);
			auto ret2 = node.get_edge_to(4);

			Assert::AreEqual((Graph::node_id)3, ret1->to);
			Assert::AreEqual(1.0, *(ret1->data));
			Assert::IsTrue(ret2 == nullptr);
		}

		TEST_METHOD(disconnect_from) 
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to((Graph::node_id)3, 1.0);
			node.connect_to((Graph::node_id)2, 0.3);

			Assert::IsTrue(node.disconnect_from(2));
			Assert::IsFalse(node.disconnect_from(1));

			auto ret1 = node.get_edge_to(2);
			auto ret2 = node.get_edge_to(3);

			Assert::IsTrue(ret1 == nullptr);
			Assert::AreEqual((Graph::node_id)3, ret2->to);
			Assert::AreEqual(1.0, *(ret2->data));
		}

		TEST_METHOD(cp_constructor)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to((Graph::node_id)3, 1.0);
			node.connect_to((Graph::node_id)2, 0.3);

			Graph::Node<double, double> node2 = node;

			auto ret1 = node2.get_edge_to(2);
			auto ret2 = node2.get_edge_to(3);

			Assert::AreEqual((Graph::node_id)2, ret1->to);
			Assert::AreEqual(0.3, *(ret1->data), precision);
			Assert::AreEqual((Graph::node_id)3, ret2->to);
			Assert::AreEqual(1.0, *(ret2->data), precision);
		}

		TEST_METHOD(is_connecting_to)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to((Graph::node_id)3, 1.0);
			node.connect_to((Graph::node_id)2, 0.3);

			node.disconnect_from(2);
			node.disconnect_from(1);
		
			Assert::IsFalse(node.is_connecting_to((Graph::node_id)2));
			Assert::IsFalse(node.is_connecting_to((Graph::node_id)1));
			Assert::IsTrue(node.is_connecting_to((Graph::node_id)3));
		}

		TEST_METHOD(get_connecting_node_list)
		{
			Graph::Node<double, double> node(1, 0.8);
			node.connect_to((Graph::node_id)3, 1.0);
			node.connect_to((Graph::node_id)2, 0.3);
			node.connect_to((Graph::node_id)5, 1.4);
			node.connect_to((Graph::node_id)4, 0.8);

			node.disconnect_from((Graph::node_id)3);
			std::list<Graph::node_id> connecting_list = node.get_connecting_node_list();

			std::list<Graph::node_id>::iterator iter = connecting_list.begin();
			Assert::AreEqual((Graph::node_id)2, *iter);
			
			iter++;
			Assert::AreEqual((Graph::node_id)4, *iter);
			
			iter++;
			Assert::AreEqual((Graph::node_id)5, *iter);

			Assert::IsTrue(++iter == connecting_list.end());
		}

		TEST_METHOD(operator_bl_eq1)
		{
			Graph::Node<double, double> node1(1, 0.7);
			Graph::Node<double, double> node2(1, 0.4);
			Assert::IsTrue(node1 == node2);
		}

		TEST_METHOD(operator_bl_eq2)
		{
			Graph::Node<double, double> node1(1, 0.7);
			Graph::Node<double, double> node2(3, 0.7);
			Assert::IsFalse(node1 == node2);
		}

		TEST_METHOD(operator_bl_neq1)
		{
			Graph::Node<double, double> node1(1, 0.7);
			Graph::Node<double, double> node2(3, 0.4);
			Assert::IsTrue(node1 != node2);
		}

		TEST_METHOD(operator_bl_neq2)
		{
			Graph::Node<double, double> node1(1, 0.7);
			Graph::Node<double, double> node2(1, 0.7);
			Assert::IsFalse(node1 != node2);
		}
		
	};
}