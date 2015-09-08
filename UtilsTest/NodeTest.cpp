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
			Node<double, Edge<BasicPathData>> node(2L, std::make_shared<double>(2.3));
			Assert::AreEqual(2.3, *node.data);
			Assert::AreEqual(2L, node.get_id());

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

		/*
		TEST_METHOD(Node_get_static_edge_to)
		{
			std::shared_ptr<Edge<BasicPathData>> edge = std::make_shared<Edge<BasicPathData>>(2.0);
			Node<Geography::LatLng, Edge<BasicPathData>> node(1L,std::make_shared<Geography::LatLng>(10.0,20.0));
			node.connect_to(edge);

			std::shared_ptr<Edge<BasicPathData> const> static_edge= node.get_static_edge_to(1L);
			
			Assert::IsTrue(edge == static_edge);

		}
		
		
		TEST_METHOD(Node_get_edge_to)
		{

		}

		TEST_METHOD(Node_connect_to)
		{

		}

		TEST_METHOD(Node_disconnect_from)
		{

		}

		TEST_METHOD(Node_is_connecting_to)
		{

		}

		TEST_METHOD(Node_get_connecting_node_list)
		{

		}

		TEST_METHOD(Node_for_each_edge)
		{

		}

	*/


	};
}
