#pragma once
#include "stdafx.h"
#include "CppUnitTest.h"
#include "Node.h"
#include "Edge.h"
#include "BasicPath.h"
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

			Node<double, BasicPath> node2(2L, std::make_shared<double>(2.3));
			Assert::AreEqual(2.3, *node2.data);
			Assert::AreEqual(2L, node2.get_id());
		}
		/*
		TEST_METHOD(Node_CopyConstructor)
		{
		Node<double, Edge<BasicPathData>> node(2L, std::make_shared<double>(2.3));
		Node<double, Edge<BasicPathData>> node2(node);

		Assert::AreEqual(2.3, *node2.data);
		Assert::AreEqual(2L, node2.get_id());

		}

		TEST_METHOD(Node_get_static_edge_to)
		{
			Node<double, Edge<BasicPathData>> node(2L, std::make_shared<double>(2.3));
			Edge<BasicPathData> edge(1L, std::make_shared<BasicPathData>(2.0));

			std::shared_ptr<Edge<BasicPathData> const> edge_pointer1 = node.get_static_edge_to(1L);
			std::shared_ptr<Edge<BasicPathData> const> edge_pointer2= node.get_static_edge_to(5L);
			Assert::IsTrue(edge == *edge_pointer1);
			Assert::IsTrue(nullptr == edge_pointer2);

		}
		*/
		TEST_METHOD(Node_)
		{

		}
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

		}*/


	};
}
