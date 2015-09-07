#include "stdafx.h"
#include "CppUnitTest.h"
#include "Node.h"
#include "Edge.h"
#include "BasicPath.h"

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

			Node<double, BasicPath> node2(2L, std::make_shared<double>(2.3));
			Assert::AreEqual(2.3, *node2.data);
			Assert::AreEqual(2L, node2.get_id());
		}

		TEST_METHOD(Node_CompileCheck) 
		{
			Node<double, Edge<BasicPathData>> node(2L, std::make_shared<double>(2.3));
			//node.connect_to(std::make_shared<Edge<BasicPathData>>(2L, std::make_shared<BasicPathData>(1.0)));

		}
		
		TEST_METHOD(Node_CopyConstructor)
		{
			Node<double, Edge<BasicPathData>> node(2L, std::make_shared<double>(2.3));
			Node<double, Edge<BasicPathData>> node2(node);
			
			//Assert::AreEqual(2.3, *node2.data);
			//Assert::AreEqual(2L, node2.get_id());
		}
		/*
		TEST_METHOD(Node_get_static_edge_to)
		{
			//std::shared_ptr<Edge<BasicPathData> const> edge = std::make_shared<double>(2.3);
			//Node<double, Edge<BasicPathData>> node(2L, edge) const; 
			
			auto edge = std::make_shared<double>(2.3);
			Node<double, Edge<BasicPathData>> node(2L, edge);

			std::shared_ptr<Edge<BasicPathData> const> static_edge1 = node.get_static_edge_to(node.get_id());
			std::shared_ptr<Edge<BasicPathData> const> static_edge2 = node.get_static_edge_to(1L);
			std::shared_ptr<Edge<BasicPathData> const> static_edge3 = node.get_static_edge_to(3L);

			//Assert::AreEqual(node., static_edge1);

		}
		*/
		/*
		TEST_METHOD(Node_)
		{

		}

		TEST_METHOD(Node_)
		{

		}

		TEST_METHOD(Node_)
		{

		}

		TEST_METHOD(Node_)
		{

		}

		TEST_METHOD(Node_)
		{

		}

		TEST_METHOD(Node_)
		{

		}

		TEST_METHOD(Node_)
		{

		}*/


	};
}
