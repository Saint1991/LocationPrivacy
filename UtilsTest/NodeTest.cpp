#include "stdafx.h"
#include "CppUnitTest.h"
#include "Node.h"
#include "Edge.h"
#include "BasicPathData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Graph;


namespace UtilsTest
{
	TEST_CLASS(NodeTest)
	{

	public:
		
		TEST_METHOD(Node_Constructor)
		{
			Node<double, Graph::Edge<Graph::BasicPathData>> node2(2L, std::make_shared<double>(2.3));
			Assert::AreEqual(2.3, *node2.data);
			Assert::AreEqual(2L, node2.get_id());
		}

		TEST_METHOD(Node_CompileCheck) 
		{
			Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>> node(3L, std::make_shared<Geography::LatLng>(2.3, 4.5));
			Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>> node2(node);
			Assert::IsTrue(node.get_static_edge_to(1L) == nullptr);
			Assert::IsTrue(node.get_edge_to(1L) == nullptr);
			Assert::IsTrue(node.connect_to(std::make_shared<Graph::Edge<Graph::BasicPathData>>(1L, std::make_shared<BasicPathData>(0.3))));
			Assert::IsTrue(node.is_connecting_to(1L));
			Assert::IsTrue(node.disconnect_from(1L));
			Assert::AreEqual(0U, node.get_connecting_node_list().size());
			node.for_each_edge([](std::shared_ptr<Graph::Edge<Graph::BasicPathData> const>) {

			});
			node.for_each_edge([](std::shared_ptr<Graph::Edge<Graph::BasicPathData>>) {

			});
		}
		
		TEST_METHOD(Node_CopyConstructor)
		{
			Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>> node(1L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			Node<Geography::LatLng, Graph::Edge<Graph::BasicPathData>> node2(node);

			Assert::AreEqual(1L, node2.get_id());
			Assert::AreEqual(10.0, node2.data->lat());
			Assert::AreEqual(20.0, node2.data->lng());
		}

		
		TEST_METHOD(Node_get_static_edge_to)
		{
			std::shared_ptr<Edge<BasicPathData>> path = std::make_shared<Edge<BasicPathData>>(2L, std::make_shared<BasicPathData>(2.0));
			Node<Geography::LatLng, Edge<BasicPathData>> node1(1L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			Node<Geography::LatLng, Edge<BasicPathData>> node2(2L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			node1.connect_to(path);

			std::shared_ptr<Edge<BasicPathData> const> static_edge1= node1.get_static_edge_to(2L);
			Assert::IsTrue(path->get_to() == static_edge1->get_to());
			Assert::IsTrue(path->get_static_data() == static_edge1->get_static_data());

			std::shared_ptr<Edge<BasicPathData> const> static_edge2 = node1.get_static_edge_to(1L);
			Assert::IsTrue(static_edge2 == nullptr);
		}
		
		
		TEST_METHOD(Node_get_edge_to)
		{
			std::shared_ptr<Edge<BasicPathData>> path = std::make_shared<Edge<BasicPathData>>(2L, std::make_shared<BasicPathData>(2.0));
			Node<Geography::LatLng, Edge<BasicPathData>> node1(1L,std::make_shared<Geography::LatLng>(10.0,20.0));
			Node<Geography::LatLng, Edge<BasicPathData>> node2(2L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			node1.connect_to(path);

			std::shared_ptr<Edge<BasicPathData>> edge1= node1.get_edge_to(2L);
			Assert::IsTrue(path->get_to() == edge1->get_to());
			Assert::IsTrue(path->get_data() == edge1->get_data());

			edge1->get_data()->update_distance(3.0);
			Assert::AreEqual(3.0, edge1->get_data()->get_distance());
			Assert::AreEqual(3.0, node1.get_edge_to(2L)->get_data()->get_distance());

			std::shared_ptr<Edge<BasicPathData>> edge2 = node1.get_edge_to(1L);
			Assert::IsTrue(edge2 == nullptr);
		}
		
		TEST_METHOD(Node_connect_to)
		{
			Node<Geography::LatLng, Edge<BasicPathData>> node1(1L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			Node<Geography::LatLng, Edge<BasicPathData>> node2(2L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			Node<Geography::LatLng, Edge<BasicPathData>> node3(3L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			std::shared_ptr<Edge<BasicPathData>> path1 = std::make_shared<Edge<BasicPathData>>(2L, std::make_shared<BasicPathData>(2.0));
			std::shared_ptr<Edge<BasicPathData>> path2 = std::make_shared<Edge<BasicPathData>>(3L, std::make_shared<BasicPathData>(2.0));
			
			Assert::IsTrue(node1.connect_to(path1));
			Assert::IsTrue(node1.connect_to(path2));
			Assert::IsFalse(node1.connect_to(path1));
			Assert::AreEqual(2U, node1.get_connecting_node_list().size());
		}
		
		TEST_METHOD(Node_disconnect_from)
		{
			Node<Geography::LatLng, Edge<BasicPathData>> node1(1L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			Node<Geography::LatLng, Edge<BasicPathData>> node2(2L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			Node<Geography::LatLng, Edge<BasicPathData>> node3(3L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			std::shared_ptr<Edge<BasicPathData>> path1 = std::make_shared<Edge<BasicPathData>>(2L, std::make_shared<BasicPathData>(2.0));
			std::shared_ptr<Edge<BasicPathData>> path2 = std::make_shared<Edge<BasicPathData>>(3L, std::make_shared<BasicPathData>(2.0));
			node1.connect_to(path1);
			node1.connect_to(path2);

			Assert::IsTrue(node1.disconnect_from(3L));
			Assert::IsFalse(node1.disconnect_from(3L));

			Assert::AreEqual(2L, *(node1.get_connecting_node_list().begin()));
			Assert::AreEqual(1U, node1.get_connecting_node_list().size());
		}

		TEST_METHOD(Node_is_connecting_to)
		{
			Node<Geography::LatLng, Edge<BasicPathData>> node1(1L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			Node<Geography::LatLng, Edge<BasicPathData>> node2(2L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			Node<Geography::LatLng, Edge<BasicPathData>> node3(3L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			std::shared_ptr<Edge<BasicPathData>> path1 = std::make_shared<Edge<BasicPathData>>(2L, std::make_shared<BasicPathData>(2.0));
			std::shared_ptr<Edge<BasicPathData>> path2 = std::make_shared<Edge<BasicPathData>>(3L, std::make_shared<BasicPathData>(2.0));
			node1.connect_to(path1);
			node1.connect_to(path2);

			Assert::IsTrue(node1.is_connecting_to(3L));
			Assert::IsFalse(node1.is_connecting_to(4L));
		}

		TEST_METHOD(Node_get_connecting_node_list)
		{
			Node<Geography::LatLng, Edge<BasicPathData>> node1(1L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			Node<Geography::LatLng, Edge<BasicPathData>> node2(2L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			Node<Geography::LatLng, Edge<BasicPathData>> node3(3L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			std::shared_ptr<Edge<BasicPathData>> path1 = std::make_shared<Edge<BasicPathData>>(3L, std::make_shared<BasicPathData>(2.0));
			std::shared_ptr<Edge<BasicPathData>> path2 = std::make_shared<Edge<BasicPathData>>(2L, std::make_shared<BasicPathData>(2.0));
			node1.connect_to(path1);
			node1.connect_to(path2);

			Assert::AreEqual(2L, *(node1.get_connecting_node_list().begin())); 
			Assert::AreEqual(3L, ++*(node1.get_connecting_node_list().begin()));
		}
		
		TEST_METHOD(Node_for_each_edge)
		{
			Node<Geography::LatLng, Edge<BasicPathData>> node1(1L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			Node<Geography::LatLng, Edge<BasicPathData>> node2(2L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			Node<Geography::LatLng, Edge<BasicPathData>> node3(3L, std::make_shared<Geography::LatLng>(10.0, 20.0));
			std::shared_ptr<Edge<BasicPathData>> path1 = std::make_shared<Edge<BasicPathData>>(3L, std::make_shared<BasicPathData>(1.0));
			std::shared_ptr<Edge<BasicPathData>> path2 = std::make_shared<Edge<BasicPathData>>(2L, std::make_shared<BasicPathData>(2.0));
			node1.connect_to(path1);
			node1.connect_to(path2);

			node1.for_each_edge([](std::shared_ptr<Edge<BasicPathData>> path) {
				path->get_data()->update_distance(path->get_data()->get_distance()+1.0);
			});

			Assert::AreEqual(2.0, path1->get_data()->get_distance());
			Assert::AreEqual(3.0, path2->get_data()->get_distance());
		}
	};
}
