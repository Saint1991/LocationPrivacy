#include "stdafx.h"
#include "CppUnitTest.h"
#include "Edge.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(EdgeTest)
	{
	public:
		
		TEST_METHOD(constructor)
		{
			Graph::node_id expected = 1;
			Graph::Edge<double> edge(1, 0.8);
			Assert::AreEqual(expected, edge.get_to());
			Assert::AreEqual(0.8, *edge.data);
		}

		TEST_METHOD(operator_bl_eq1)
		{
			Graph::node_id to = 3;
			Graph::Edge<double> edge1(to, 0.6);
			Graph::Edge<double> edge2(to, -0.6);
			Assert::IsTrue(edge1 == edge2);
		}

		TEST_METHOD(operator_bl_eq2)
		{
			Graph::Edge<double> edge1(3, 0.6);
			Graph::Edge<double> edge2(2, 0.6);
			Assert::IsFalse(edge1 == edge2);
		}

		TEST_METHOD(operator_bl_neq1)
		{
			Graph::Edge<double> edge1(3, 0.6);
			Graph::Edge<double> edge2(2, 0.6);
			Assert::IsTrue(edge1 != edge2);
		}

		TEST_METHOD(operator_bl_neq2)
		{
			Graph::Edge<double> edge1(2, 0.7);
			Graph::Edge<double> edge2(2, 0.6);
			Assert::IsFalse(edge1 != edge2);
		}

	};
}